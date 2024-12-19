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

using Ookii.CommandLine.Commands;
using System.Text;
using System;

namespace bnrtool.Options
{
    [GeneratedCommandManager]
    internal sealed partial class GlobalCommandManager
    {
        private static GlobalCommandManager? instance = null;

        internal static GlobalCommandManager Instance
            => instance ?? throw new InvalidOperationException($"{typeof(GlobalCommandManager).Name} not initialized");

        internal required Func<ICommand, int> Runner { get; init; }

        internal required int ErrorStatus { get; init; }

        internal required Encoding Encoding { get; init; }

        internal static GlobalCommandManager Initialize(Func<ICommand, int> runner, int errorStatus, Encoding encoding,
            CommandOptions? options = null) => instance ??= new GlobalCommandManager(options)
            {
                Runner = runner,
                ErrorStatus = errorStatus,
                Encoding = encoding
            };

        internal int Run(string[] args)
        {
            Console.OutputEncoding = Encoding;
            return RunCommand(args) ?? ErrorStatus;
        }

        public int Invoke(ICommand command) => Runner.Invoke(command);
    }
}
