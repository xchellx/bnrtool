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
    [Command("encode")]
    [Description("Encode a PNG image file and a YAML info file into a BNR file.")]
    [ParseOptions(
        AutoHelpArgument = true,
        AutoVersionArgument = false,
        IsPosix = true,
        Mode = ParsingMode.LongShort,
        DuplicateArguments = ErrorMode.Error
    )]
    internal sealed partial class EncodeCommandOptions : GlobalCommandOptions
    {
        [CommandLineArgument(argumentName: "image", Position = 0)]
        [Description("Input PNG image file to read from.")]
        public required string InputImageFile { get; set; }

        [CommandLineArgument(argumentName: "info", Position = 1)]
        [Description("Input YAML info file to read from.")]
        public required string InputInfoFile { get; set; }

        [CommandLineArgument(argumentName: "bnr", Position = 2)]
        [Description("Output BNR file to write to.")]
        public required string OutputFile { get; set; }
    }
}
