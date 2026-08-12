# coco

USB conversion for a real TRS-80 Color Computer 3 keyboard, using a custom PCB
that wires the CoCo keyboard's matrix connector directly to a Raspberry Pi
Pico (RP2040). The CoCo keyboard is a diode-less matrix, so pressing certain
combinations of 3+ keys at once can produce ghost keypresses — this is
inherent to the original hardware, not a firmware bug.

* Keyboard Maintainer: William Athing
* Hardware Supported: TRS-80 CoCo 3 keyboard + custom Pico interposer PCB
* Hardware Availability: Personal/one-off build

Make example for this keyboard (after setting up your build environment):

    make coco:default

Flashing example for this keyboard:

    make coco:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
