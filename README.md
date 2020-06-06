# Window Hijack
Rendering on external windows via hijacking thread contexts.
You can read a full write-up at: https://secret.club/2020/05/12/abusing-compositions.html

## Usage
If you plan on manual mapping this driver, or using it on a game with an anti-cheat then you'll need to make some modifications, these are left for you to do.

To use the project as-is:

1. Enable test-signing mode
2. Compile in Release x64
3. Load the driver using any traditional driver loader
4. Start window_hijack_cli using the following pattern: `window_hijack_cli.exe [window_class_name]`

## Credits
paracord (yousif), IChooseYou
