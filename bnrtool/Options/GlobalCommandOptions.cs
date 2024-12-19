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
    internal abstract class GlobalCommandOptions : ICommand
    {
        [CommandLineArgument(argumentName: "silent", ShortName = 's')]
        [Description("Do not print anything to console.")]
        public bool Silent { get; set; } = false;

        [CommandLineArgument(argumentName: "nooutput")]
        [Description("Do not print output to console.")]
        public bool DontPrintOutput { get; set; } = false;

        [CommandLineArgument(argumentName: "noerrors")]
        [Description("Do not print errors to console.")]
        public bool DontPrintErrors { get; set; } = false;

        [CommandLineArgument(argumentName: "force", ShortName = 'f')]
        [Description("Do not ask to overwrite file output(s).")]
        public bool Force { get; set; } = false;

        [CommandLineArgument(argumentName: "tga", ShortName = 't')]
        [Description("Load a TGA image instead of a PNG image.")]
        public bool UseTGA { get; set; } = false;

        [CommandLineArgument(argumentName: "japan", ShortName = 'j')]
        [Description("Enable ShiftJIS text encoding for BNR1 files. This option is ignored for BNR2 files.")]
        public bool JapanEncoding { get; set; } = false;

        public int Run() => GlobalCommandManager.Instance.Invoke(this); 
    }
}
