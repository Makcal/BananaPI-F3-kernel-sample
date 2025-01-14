# Kernel sample for BananaPI F3

BananaPI F3 is a multifunctional single-board microcomputer with RISC-V CPU.
This repository demonstrates how you can flash a board with a custom executable (i.e. avoiding an OS).
The flashing process is a compilation of several guides and the official documentation.
The key idea is to use U-Boot and spoof a Linux kernel. Sorry, I don't yet know a better way.

## Useful links
- [Poor board's site with some info and guides](https://docs.banana-pi.org/en/BPI-F3/BananaPi_BPI-F3)
- [CPU documentation](https://developer.spacemit.com/documentation)
- [Instruction on a basic flashing procedure](https://github.com/jellyterra/bpi-f3-archlinux)
- [First part of a trilogy about U-Boot](https://krinkinmu.github.io/2023/08/12/getting-started-with-u-boot.html)

## Steps
1. Download and unpack the
[cross-compilation toolchain](https://developer.spacemit.com/documentation?token=BZfYwFfEmiSr7dkintEca9ERn3b). I use GCC toolchain in this guide.

2. Add the `bin/` folder of the downloaded archive to the PATH:
```bash
export PATH=<PATH_TO_TOOLCHAIN_BIN>/bin:$PATH
```

3. Run `make` command. Instead of using PATH, you can pass the path as `xc_bin` variable to `make`:
```bash
xc_bin=<PATH_TO_TOOLCHAIN_BIN>/bin make
```

4. You can observe a spoofed Linux kernel in the `build/` folder.
Put it to the `bootfs.ext4` similar to spoofing `rootfs.ext4`
that is described in the flashing instruction from "Useful links".
\
If you have mounted `bootfs.ext4` to `/mnt`, then you can just call `make transfer`.

5. **[Optional]** If for some reason you want to keep the original Linux kernel in place, there is `make append`
that will append your binary after the kernel from variable `true_linux_path`. Note, that you will need
to modify the kernel's header to pass control to your binary.

