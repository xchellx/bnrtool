/*
 * MIT License
 * 
 * Copyright (c) 2023 Yonder
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

using System;
using System.IO;
using System.Text;
using Ookii.CommandLine.Commands;
using Ookii.CommandLine;
using bnrtool.Options;
using UtilSharp.Util;
using UtilSharp.IO;
using SerializableSharp;
using BNRSharp.Serialization;
using BNRSharp;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace bnrtool
{
    public static class Program
    {
        public static TextWriter StdOut { get; private set; } = Console.Out;

        public static TextWriter StdErr { get; private set; } = Console.Error;

        public static int Main(string[] args) => GlobalCommandManager.Initialize(
            OnRun,
            (int) Status.Error,
            Encoding.UTF8,
            new CommandOptions()
            {
                AutoHelpArgument = true,
                AutoVersionArgument = false,
                AutoVersionCommand = true,
                IsPosix = true,
                Mode = ParsingMode.LongShort,
                DuplicateArguments = ErrorMode.Error,
                Error = StdErr,
                ForceReflection = false,
                ShowUsageOnError = UsageHelpRequest.SyntaxOnly,
                UseErrorColor = false,
                UsageWriter = new UsageWriter(new LineWrappingTextWriter(Console.Error, Console.BufferWidth,
                    disposeBaseWriter: false), useColor: false)
                {
                    // Ookii.CommandLine.CommandLineParser.GetExecutableName(Boolean):
                    // 'System.Reflection.Assembly.Location.get' always returns an empty string for assemblies embedded
                    // in a single-file app.
                    ExecutableName = "bnrtool",
                    IncludeExecutableExtension = false,
                    IncludeApplicationDescription = true,
                    IncludeApplicationDescriptionBeforeCommandList = true,
                    IncludeDefaultValueInDescription = true,
                    IncludeCommandHelpInstruction = true,
                }
            }
        ).Run(args);

        private static int OnRun(ICommand options)
        {
            // Extra encodings
            Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);

            if (options is GlobalCommandOptions gOptions)
            {
                if (gOptions.Silent || gOptions.DontPrintOutput)
                    StdOut = TextWriter.Null;
                if (gOptions.Silent || gOptions.DontPrintErrors)
                    StdErr = TextWriter.Null;

                if (gOptions is DecodeCommandOptions unpackOptions)
                    return (int) OnDecode(unpackOptions);
                else if (gOptions is EncodeCommandOptions packOptions)
                    return (int) OnEncode(packOptions);
            }

            Console.Error.WriteLine("ERROR: Unexpected options class {0}", options.GetType().Name);
            return (int) Status.Error;
        }

        private static Status OnDecode(DecodeCommandOptions options)
        {
            if (CLIUtil.CheckFileArgument(
                path: options.InputFile,
                preSanitizer: s => s.Trim(),
                postSanitizer: Path.GetFullPath,
                sanitizedPath: out string inputFile,
                isOutput: false,
                forceOverwrite: false,
                console: StdErr,
                failTypeMsg: $"ERROR: Failed to get file type of \"{options.InputFile}\": {{0}}",
                notFileMsg: $"ERROR: \"{options.InputFile}\" must be a file.",
                notExistMsg: $"ERROR: \"{options.InputFile}\" does not exist.",
                noOvrwrtMsg: string.Empty
            ))
                return Status.ArgError;

            if (CLIUtil.CheckFileArgument(
                path: options.OutputInfoFile,
                preSanitizer: s => s.Trim(),
                postSanitizer: Path.GetFullPath,
                sanitizedPath: out string outputInfoFile,
                isOutput: true,
                forceOverwrite: options.Force || options.ForceInfo,
                console: StdErr,
                failTypeMsg: $"ERROR: Failed to get file type of \"{options.OutputInfoFile}\": {{0}}",
                notFileMsg: $"ERROR: \"{options.OutputInfoFile}\" must be a file.",
                notExistMsg: $"\"{options.OutputInfoFile}\" already exists. Do you want to overwrite it?",
                noOvrwrtMsg: $"ERROR: Cannot overwrite \"{options.OutputInfoFile}\"; user choose not to overwrite it."
            ))
                return Status.ArgError;

            if (CLIUtil.CheckFileArgument(
                path: options.OutputImageFile,
                preSanitizer: s => s.Trim(),
                postSanitizer: Path.GetFullPath,
                sanitizedPath: out string outputImageFile,
                isOutput: true,
                forceOverwrite: options.Force || options.ForceImage,
                console: StdErr,
                failTypeMsg: $"ERROR: Failed to get file type of \"{options.OutputImageFile}\": {{0}}",
                notFileMsg: $"ERROR: \"{options.OutputImageFile}\" must be a file.",
                notExistMsg: $"\"{options.OutputImageFile}\" already exists. Do you want to overwrite it?",
                noOvrwrtMsg: $"ERROR: Cannot overwrite \"{options.OutputImageFile}\"; user choose not to overwrite it."
            ))
                return Status.ArgError;

            bool isBnr2;
            Encoding encoding;

            IBNR bnr;
            FileStream? bnrStream = null;
            EndianBinaryReader? bnrReader = null;
            try
            {
                bnrStream = File.OpenRead(inputFile);
                bnrReader = new EndianBinaryReader(bnrStream, isLittleEndian: false, encoding: IOUtil.Windows1252,
                    leaveOpen: true);

                isBnr2 = new BNR().CheckMagic(m => m is string s && BNRHandler.IsMagicBNR2(s), bnrStream, bnrReader);
                encoding = !isBnr2 && options.JapanEncoding ? IOUtil.ShiftJIS : IOUtil.Windows1252;

                bnrReader.Seek(0, SeekOrigin.Begin);
                bnrReader.Dispose();
                bnrReader = new EndianBinaryReader(bnrStream, isLittleEndian: false, encoding, leaveOpen: true);

                ISerializable? bnrVer = new BNR().Read(bnrStream, bnrReader);
                if (bnrVer is IBNR bnrVerSpec)
                {
                    bnr = bnrVerSpec;
                    bnr.Read(bnrStream, bnrReader);
                }
                else
                    throw new InvalidOperationException("BNR reader did not return a version specific reader");
            }
            catch (SerializationException e)
            {
                StdErr.WriteLine("ERROR: Failed to read BNR: {0} {1}", e, e.InnerException?.ToString() ??
                    string.Empty);
                return Status.IOError;
            }
            catch (Exception e)
            {
                StdErr.WriteLine("ERROR: Unexpected error when reading BNR: {0} {1}", e, e.InnerException?.ToString()
                    ?? string.Empty);
                return Status.IOError;
            }
            finally
            {
                bnrReader?.Dispose();
                bnrStream?.Dispose();
            }

            FileStream? imageStream = null;
            try
            {
                imageStream = File.OpenWrite(outputImageFile);
                imageStream.SetLength(0);
                BNRHandler.DecodeImage(bnr, options.UseTGA, options.CompressImage, imageStream);
            }
            catch (Exception e)
            {
                StdErr.WriteLine("ERROR: Failed to decode BNR image: {0} {1}", e, e.InnerException?.ToString()
                    ?? string.Empty);
                return Status.FmtError;
            }
            finally
            {
                imageStream?.Dispose();
            }

            FileStream? infoStream = null;
            try
            {
                infoStream = File.OpenWrite(outputInfoFile);
                infoStream.SetLength(0);
                BNRHandler.DecodeInfo(isBnr2, bnr, encoding, infoStream);
            }
            catch (Exception e)
            {
                StdErr.WriteLine("ERROR: Failed to decode BNR info: {0} {1}", e, e.InnerException?.ToString()
                    ?? string.Empty);
                return Status.FmtError;
            }
            finally
            {
                infoStream?.Dispose();
            }

            return Status.Success;
        }

        private static Status OnEncode(EncodeCommandOptions options)
        {
            if (CLIUtil.CheckFileArgument(
                path: options.InputImageFile,
                preSanitizer: s => s.Trim(),
                postSanitizer: Path.GetFullPath,
                sanitizedPath: out string inputImageFile,
                isOutput: false,
                forceOverwrite: false,
                console: StdErr,
                failTypeMsg: $"ERROR: Failed to get file type of \"{options.InputImageFile}\": {{0}}",
                notFileMsg: $"ERROR: \"{options.InputImageFile}\" must be a file.",
                notExistMsg: $"ERROR: \"{options.InputImageFile}\" does not exist.",
                noOvrwrtMsg: string.Empty
            ))
                return Status.ArgError;

            if (CLIUtil.CheckFileArgument(
                path: options.InputInfoFile,
                preSanitizer: s => s.Trim(),
                postSanitizer: Path.GetFullPath,
                sanitizedPath: out string inputInfoFile,
                isOutput: false,
                forceOverwrite: false,
                console: StdErr,
                failTypeMsg: $"ERROR: Failed to get file type of \"{options.InputInfoFile}\": {{0}}",
                notFileMsg: $"ERROR: \"{options.InputInfoFile}\" must be a file.",
                notExistMsg: $"ERROR: \"{options.InputInfoFile}\" does not exist.",
                noOvrwrtMsg: string.Empty
            ))
                return Status.ArgError;

            if (CLIUtil.CheckFileArgument(
                path: options.OutputFile,
                preSanitizer: s => s.Trim(),
                postSanitizer: Path.GetFullPath,
                sanitizedPath: out string outputFile,
                isOutput: true,
                forceOverwrite: options.Force,
                console: StdErr,
                failTypeMsg: $"ERROR: Failed to get file type of \"{options.OutputFile}\": {{0}}",
                notFileMsg: $"ERROR: \"{options.OutputFile}\" must be a file.",
                notExistMsg: $"\"{options.OutputFile}\" already exists. Do you want to overwrite it?",
                noOvrwrtMsg: $"ERROR: Cannot overwrite \"{options.OutputFile}\"; user choose not to overwrite it."
            ))
                return Status.ArgError;

            bool isBnr2;
            BNR bnrHead = new();

            FileStream? checkStream = null;
            try
            {
                checkStream = File.OpenRead(inputInfoFile);
                isBnr2 = BNRHandler.IsInfoBNR2(bnrHead, checkStream);
            }
            catch (Exception e)
            {
                StdErr.WriteLine("ERROR: Failed to detect BNR format: {0}", e);
                return Status.FmtError;
            }
            finally
            {
                checkStream?.Dispose();
            }

            IBNR bnr = isBnr2 ? new BNR2(bnrHead) : new BNR1(bnrHead);
            Encoding encoding = !isBnr2 && options.JapanEncoding ? IOUtil.ShiftJIS : IOUtil.Windows1252;

            FileStream? imageStream = null;
            try
            {
                imageStream = File.OpenRead(inputImageFile);
                BNRHandler.EncodeImage(bnr, options.UseTGA, imageStream);
            }
            catch (Exception e)
            {
                StdErr.WriteLine("ERROR: Failed to encode BNR image: {0} {1}", e, e.InnerException?.ToString()
                    ?? string.Empty);
                return Status.FmtError;
            }
            finally
            {
                imageStream?.Dispose();
            }

            FileStream? infoStream = null;
            try
            {
                infoStream = File.OpenRead(inputInfoFile);
                BNRHandler.EncodeInfo(isBnr2, bnr, encoding, infoStream);
            }
            catch (Exception e)
            {
                StdErr.WriteLine("ERROR: Failed to encode BNR info: {0} {1}", e, e.InnerException?.ToString()
                    ?? string.Empty);
                return Status.FmtError;
            }
            finally
            {
                infoStream?.Dispose();
            }

            FileStream? bnrStream = null;
            EndianBinaryWriter? bnrWriter = null;
            try
            {
                bnrStream = File.OpenWrite(outputFile);
                bnrStream.SetLength(0);
                bnrWriter = new EndianBinaryWriter(bnrStream, isLittleEndian: false, encoding, leaveOpen: true);
                bnrHead.Write(bnrStream, bnrWriter, versionSpec: bnr);
            }
            catch (SerializationException e)
            {
                StdErr.WriteLine("ERROR: Failed to write BNR: {0} {1}", e, e.InnerException?.ToString()
                    ?? string.Empty);
                return Status.IOError;
            }
            catch (Exception e)
            {
                StdErr.WriteLine("ERROR: Unexpected error when writing BNR: {0} {1}", e, e.InnerException?.ToString()
                    ?? string.Empty);
                return Status.IOError;
            }
            finally
            {
                bnrWriter?.Dispose();
                bnrStream?.Dispose();
            }

            return Status.Success;
        }
    }
}
