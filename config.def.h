/* See LICENSE file for copyright and license details. */

#include "tcl.c"

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const double defaultopacity  = 1;
static const int horizpadbar        = 0;        /* horizontal padding for statusbar */
static const int vertpadbar         = 4;        /* vertical padding for statusbar */
static const int layoutspace        = 15;       /* distance between the tags and the layout icon */
static const char *fonts[]          = { "Ricty Discord:size=12.5" };
/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
enum showtab_modes { showtab_always, showtab_never, showtab_nmodes, showtab_auto};
static const int showtab			= showtab_always;        /* Default tab bar show mode */
static const int toptab				= False;                 /* False means bottom tab bar */
static const int centertab			= True;
static const int padtab             = 60;                    /* lR padding for tab windows */ // 30
static const char *def_tab_icon     = "\uf111";      /* default tab icon*/
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red_bright[]  = "#BD335E";
static const char col_red[]         = "#822B45";
static const char col_purple_light[]= "#564c69";
static const char col_purple_dark[] = "#3C2454";
static const char col_yellow[]      = "#a3a337";
static const unsigned int baralpha = 0xc3;      /* alpha range: 00 - ff */
static const unsigned int baralphasel = 0xfb;
static const unsigned int borderalpha = 0x8f;
static const char *colors[][5]      = {
	/*               fg          bg                border             float border     icon fg   */
	[SchemeNorm] = { col_gray3,  col_gray1,        col_purple_light,  col_gray3,       col_yellow },
	[SchemeSel]  = { col_gray4,  col_purple_dark,  col_red,           col_red_bright,  col_yellow },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg           border     */
	[SchemeNorm] = { OPAQUE, baralpha   , borderalpha },
	[SchemeSel]  = { OPAQUE, baralphasel, borderalpha },
};

/* terminal emulator */
static const char terminal[] = "kitty";

/* tagging */
static const char *tags[] = {
    "1 \uecc8", // terminal
    "2 \uebde", // browser (personal)
    "3 \uec24", // ide
    "4 \ue970", // file browser
    "5 \uf15c", // text editor
    "6 \uf001", // music
    "7 \uf069", // misc
    "8 \uf27a", // communication
    "9 \uea2c"  // browser (work)
};

/* default layout per tags */
/* The first element is for all-tag view, following i-th element corresponds to */
/* tags[i]. Layout is referred using the layouts array index.*/
static int def_layouts[1 + LENGTH(tags)]  = { 4, 0, 2, 0, 0, 0, 0, 0, 0, 0};

/* tag assignment and floating rules */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                       instance    title      tab icon       tags mask     isfloating   monitor    float x,y,w,h         floatborderpx*/

	// tag assignment
    { "firefox",                   NULL,       NULL,      "\uf269",      1 << 1,       0,           -1,        50,50,500,500,        8 },
    { "code-oss",                  NULL,       NULL,      NULL,          1 << 2,       0,           -1,        50,50,500,500,        8 },
    { "Thunar",                    NULL,       NULL,      NULL,          1 << 3,       0,           -1,        50,50,500,500,        8 },
	{ "Emacs",                     NULL,       NULL,      "\uf1b2",      1 << 4,       0,           -1,        50,50,500,500,        8 },
    { "Subl3",                     NULL,       NULL,      NULL,          1 << 4,       0,           -1,        50,50,500,500,        8 },
    { terminal,                   "ncmpcpp",   NULL,      NULL,          1 << 5,       0,           -1,        50,50,500,500,        8 },
    { "youtube-music-desktop-app", NULL,       NULL,      NULL,          1 << 5,       0,           -1,        50,50,500,500,        8 },
    { "mpv",                       NULL,       NULL,      NULL,          1 << 6,       0,           -1,        50,50,500,500,        8 },
    { "zoom",                      NULL,       NULL,      NULL,          1 << 7,       0,           -1,        50,50,500,500,        8 },

	// floating rules
    { "Gimp",                      NULL,       NULL,      NULL,          0,            1,           -1,        50,50,500,1000,       8 },
    { "Ahoviewer",                 NULL,       NULL,      NULL,          0,            1,           -1,        50,50,1500,1000,      8 },
    { "Hachoir-metadata-gtk",      NULL,       NULL,      NULL,          0,            1,           -1,        50,50,500,500,        8 },
    { "Gcolor3",                   NULL,       NULL,      NULL,          0,            1,           -1,        50,50,500,500,        8 },
    { "Gnome-calculator",          NULL,       NULL,      NULL,          0,            1,           -1,        50,50,500,500,        8 },
    { "Android Emulator",          NULL,       NULL,      NULL,          0,            1,           -1,        50,50,500,500,        8 },
    { "Blueberry.py",              NULL,       NULL,      NULL,          0,            1,           -1,        50,50,500,500,        8 },
    { "Zoom Group Chat",           NULL,       NULL,      NULL,          0,            1,           -1,        50,50,500,500,        8 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T \uf0c9",     tile },    /* first entry is default */
	{ "F \ue965",     NULL },    /* no layout function means floating behavior */
	{ "M [M]",        monocle },
	{ "D [D]",        deck },
	{ "G \uf00a",     grid },
	{ "C \uea99",     tcl },
	{ NULL,           NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { terminal, NULL };
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *roficmd[] = { "rofi", "-show", "combi", NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { terminal, "--title", scratchpadname, NULL };

static const char *cmd_rofi_refresh_files[] = { "/home/kyukee/Scripts/fmenu-rofi.sh", "-f", NULL };
static const char *cmd_files[]  = { "thunar", NULL };
static const char *cmd_files_terminal[]  = { terminal, "--name", "ranger", "-e", "ranger", NULL };
static const char *cmd_music_terminal[]  = { terminal, "--name", "ncmpcpp", "-e", "ncmpcpp", NULL };
static const char *cmd_browser[]  = { "firefox", NULL };
static const char *cmd_text_editor[]  = { "emacs", NULL };
static const char *cmd_ide[]  = { "code-oss", NULL };
static const char *cmd_lock[]  = { "i3lock", "--blur=10", "--composite", "--clock", "--timecolor=ffffffff", "--timesize=90", "--datecolor=ffffffff", "--datesize=24", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	//{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_space,  spawn,          {.v = roficmd } },
    { MODKEY|ShiftMask,             XK_d,      spawn,          {.v = cmd_rofi_refresh_files } },
    { MODKEY,                       XK_e,      spawn,          {.v = cmd_files } },
    { MODKEY|ShiftMask,             XK_e,      spawn,          {.v = cmd_files_terminal } },
    { MODKEY,                       XK_n,      spawn,          {.v = cmd_music_terminal } },
    { MODKEY,                       XK_b,      spawn,          {.v = cmd_browser } },
    { MODKEY,                       XK_u,      spawn,          {.v = cmd_text_editor } },
    { MODKEY,                       XK_i,      spawn,          {.v = cmd_ide } },
    { MODKEY|ControlMask,           XK_l,      spawn,          {.v = cmd_lock } },
	{ MODKEY|ControlMask,           XK_s,      spawn,          SHCMD("transset-df -a --dec .1") },
	{ MODKEY|ControlMask,           XK_d,      spawn,          SHCMD("transset-df -a --inc .1") },
	{ MODKEY|ControlMask,           XK_f,      spawn,          SHCMD("transset-df -a .75") },
	{ MODKEY|ControlMask,           XK_Return, togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_w,      tabmode,        {-1} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_plus,   incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ Mod1Mask,                     XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
    { MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	// { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	// { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	// { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	// { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	// { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	//{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_Left,   cyclelayout,    {.i = -1 } },
	{ MODKEY,                       XK_Right,  cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
    { MODKEY,                       XK_s,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY,                       XK_Tab,    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_7,      setborderpx,    {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      setborderpx,    {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_9,      setborderpx,    {.i = 0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ControlMask,           XK_e,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} },
	{ MODKEY,                       XK_o,      winview,        {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
};
