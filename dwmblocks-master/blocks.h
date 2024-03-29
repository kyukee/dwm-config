//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
    /*Icon*/	        /*Command*/		                        /*Update Interval*/	  /*Update Signal*/

    {"", "~/Development/dwm/dwmblocks-master/bar-functions/cpu_load.sh",               2,                0},

    {"", "~/Development/dwm/dwmblocks-master/bar-functions/cpu_temp.sh",               2,                0},

    {"", "~/Development/dwm/dwmblocks-master/bar-functions/date.sh",                  30,                0},

    {"", "~/Development/dwm/dwmblocks-master/bar-functions/clock.sh",                  5,                0},

    {"", "~/Development/dwm/dwmblocks-master/bar-functions/battery.sh",               10,                0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char* delim = "     ";
static unsigned int delimLen = 5;
