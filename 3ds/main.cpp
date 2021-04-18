/*
	Hello World example made by Aurelio Mannara for ctrulib
	This code was modified for the last time on: 12/12/2014 21:00 UTC+1

	This wouldn't be possible without the amazing work done by:
	-Smealum
	-fincs
	-WinterMute
	-yellows8
	-plutoo
	-mtheall
	-Many others who worked on 3DS and I'm surely forgetting about
*/

#include <3ds.h>
#include <stdio.h>
#include <yac8emul/yac8emul.h>
#include <yac8emul/interpreter.h>

int main(int argc, char **argv)
{
  yac8emul::cpu cpu{};
  std::vector<std::uint8_t> rom {
      0x12, 0x06, // jmp +4
      0x00, 0x00, 0x00, 0x00,
      0x6A, 0x23, // VA = 0x23
      0x6B, 0x24, // VB = 0x24
      0x9A, 0xB0, // VA != VB?
      0x6A, 0x99, // VA = 0x99 -- this shouldn't happen since VA *is different* from
              // VB.
      0x6C, 0xFF, // VC = 0xFF
      0x5A, 0xB0, // Skip if VA == VB?
      0x6D, 0xFF, 0x6E, 0xFF,
  };
  cpu.load_rom(rom);
  char buffer[100] = {};
  

	gfxInitDefault();

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, NULL);

	//Move the cursor to row 15 and column 19 and then prints "Hello World!"
	//To move the cursor you have to print "\x1b[r;cH", where r and c are respectively
	//the row and column where you want your cursor to move
	//The top screen has 30 rows and 50 columns
	//The bottom screen has 30 rows and 40 columns
  
	printf("\x1b[15;19HHello World!");

	printf("\x1b[29;15HPress Start to exit.");

  int i = 0;
  bool should_print = false;
  bool finished = false;
	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();

    try {
      cpu.execute_instruction();
    } catch (yac8emul::errors::invalid_instruction& e) {
      sprintf(buffer, "\x1b[%d;%dH%s", i++, 0, e.get_info());
      should_print = true;
    } catch (yac8emul::errors::not_implemented& e) {
      sprintf(buffer, "\x1b[%d;%dH%s", i++, 0, e.get_info());
      should_print = true;
    } catch (std::invalid_argument& e) {
      sprintf(buffer, "\x1b[%d;%dH%s", i++, 0, "finished");
      should_print = true;
      finished = true;
    } 
    i = i % 30;

    if (should_print && !finished) {
      printf("%s", buffer);
    }
	}

	gfxExit();
	return 0;
}
