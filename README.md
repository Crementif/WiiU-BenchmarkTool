# Wii U benchmark

A simple homebrew application to benchmark various things like the CPU, storage and memory on a Wii U. Works by running a bunch of different algorithms and then printing out the duration.

#### Usage

The .elf file can be run on a Wii U via Homebrew Launcher or in Cemu by launching it directly.

#### How to compile

1. Get DevkitPro PPC
2. Open BenchmarkTool.pnproj in Programmer's Notepad
3. Run `make` in your command prompt to compile

Optional: Add Make as a shortcut in Programmer's Notepad
4. Click Tools -> Options, then navigate to Project Tools.
5. Click on Add and fill the fields in [like this.](https://i.imgur.com/EYk3VjJ.png)
6. Now, you should be able to compile if you press F5 or if you go to Tools -> Compile