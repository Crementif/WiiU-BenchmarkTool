# Wii U benchmark

A simple homebrew application to benchmark various things like the CPU, storage and memory on a Wii U. Works by running a bunch of different algorithms and then printing out the duration.

#### Usage

The .elf file can be run on a Wii U via Homebrew Launcher or in Cemu by launching it directly.

#### How to compile

1. Get DevkitPro PPC (make sure it sets the path correctly)
2. Run the `make` command in your command prompt while being in the project root directory.

#### How to make changes (with Visual Studio)

1. Open the "Wii U Benchmark.sln" file to open the project.
2. Optionally, you can use the Debug functionality of VS if you have a Cemu folder in the project root with a Cemu installation in it.
3. Additionally, if you want to get intellisense to work, you'll need to insert your own PPC paths by going to `Project`->`Properties`->`Nmake` and changing the Include Search Path with your own DevkitPro location.