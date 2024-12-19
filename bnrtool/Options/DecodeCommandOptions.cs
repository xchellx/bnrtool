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

using Ookii.CommandLine;
using Ookii.CommandLine.Commands;
using System.ComponentModel;

namespace bnrtool.Options
{
    [GeneratedParser]
    [Command("decode")]
    [Description("Decodes a BNR file into a PNG image file and a YAML info file.")]
    [ParseOptions(
        AutoHelpArgument = true,
        AutoVersionArgument = false,
        IsPosix = true,
        Mode = ParsingMode.LongShort,
        DuplicateArguments = ErrorMode.Error
    )]
    internal sealed partial class DecodeCommandOptions : GlobalCommandOptions
    {
        [CommandLineArgument(argumentName: "bnr", Position = 0)]
        [Description("Input BNR file to read from.")]
        public required string InputFile { get; set; }

        [CommandLineArgument(argumentName: "image", Position = 1)]
        [Description("Output PNG image file to write to.")]
        public required string OutputImageFile { get; set; }

        [CommandLineArgument(argumentName: "info", Position = 2)]
        [Description("Output YAML info file to write to.")]
        public required string OutputInfoFile { get; set; }

        [CommandLineArgument(argumentName: "forceimage")]
        [Description("Do not ask to overwrite PNG image file output.")]
        public bool ForceImage { get; set; } = false;

        [CommandLineArgument(argumentName: "forceinfo")]
        [Description("Do not ask to overwrite YAML info file output.")]
        public bool ForceInfo { get; set; } = false;

        [CommandLineArgument(argumentName: "compress", ShortName = 'c')]
        [Description("Compress the output PNG image file output.")]
        public bool CompressImage { get; set; } = false;
    }
}
