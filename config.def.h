/* See LICENSE file for copyright and license details. */

#include "tcl.c"
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 5;            /* border pixel of windows */
static const unsigned int snap      = 32;           /* snap pixel */
static const unsigned int systraypinning = 0;       /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 8;       /* systray spacing */
static const int systraypinningfailfirst = 1;       /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;            /* 0 means no systray */
static const unsigned int gappih    = 10;           /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;           /* vert inner gap between windows */
static const unsigned int gappoh    = 10;           /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;           /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;            /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;            /* 0 means no bar */
static const int topbar             = 1;            /* 0 means bottom bar */
static const int iscentered_default = 1;            /* 1 means floating windows are centered by default */
static const double defaultopacity  = 1;
static const int horizpadbar        = 0;            /* horizontal padding for statusbar */
static const int vertpadbar         = 4;            /* vertical padding for statusbar */
static const int layoutspace        = 15;           /* distance between the tags and the layout icon */
static const char *fonts[]          = { "Ricty Discord:size=12.5" };
/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
enum showtab_modes { showtab_always, showtab_never, showtab_nmodes, showtab_auto};
static const int showtab            = showtab_always;       /* Default tab bar show mode */
static const int toptab             = False;                /* False means bottom tab bar */
static const int centertab          = True;
static const int padtab             = 60;                   /* lR padding for tab windows */ // 30
static const char *def_tab_icon     = "\uf111";             /* default tab icon*/
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red_bright[]  = "#33bdb0";
static const char col_red[]         = "#822B45";
static const char col_purple_light[]= "#564c69";
static const char col_purple_dark[] = "#3C2454";
static const char col_yellow[]      = "#a3a337";
static const unsigned int baralpha = 0xc3;          /* alpha range: 00 - ff */
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
	"1 \uecc8",   /* terminal */
	"2 \uebde",   /* browser (personal) */
	"3 \uf15c",   /* programming */
	"4 \uea2c",   /* browser (work) */
	"5 \ue970",   /* file browser */
	"6 \ue92c",   /* video */
	"7 \uf001",   /* music */
	"8 \uf27a",   /* communication */
	"9 \uf069",   /* misc */
};

/* default layout per tags */
/* The first element is for all-tag view, following i-th element corresponds to */
/* tags[i]. Layout is referred using the layouts array index.*/
static int def_layouts[1 + LENGTH(tags)]  = { 4, 0, 2, 2, 2, 2, 2, 2, 2, 2};

/* layout(s) */
static const float mfact     = 0.6;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol         arrange function */
	{ "T \uf0c9",     tile },    /* first entry is default */
	{ "F \ue965",     NULL },    /* no layout function means floating behavior */
	{ "M \uf2d0",     monocle },
	{ "D \uebc7",     deck },
	{ "G \uf00a",     grid },
	{ "C \uea99",     tcl },
	{ NULL,           NULL },
};

/* xprop(1):
	 *  WM_CLASS(STRING) = instance, class
	 *  WM_NAME(STRING) = title
	 */

/* tag assignment and floating rules */
static const Rule rules[] = {
	/* class                       instance    title      tab icon   tags mask    monitor   canfocus   isfloating   iscentered   float x,y,w,h      floatborderpx*/

	// tag assignment
	{ "firefox",                   NULL,       NULL,     "\uf269",   1 << 1,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "code-oss",                  NULL,       NULL,      NULL,      1 << 2,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "Emacs",                     NULL,       NULL,     "\uf1b2",   1 << 2,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "Subl3",                     NULL,       NULL,      NULL,      1 << 2,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "Nemo",                      NULL,       NULL,      NULL,      1 << 4,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "mpv",                       NULL,       NULL,      NULL,      1 << 5,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ terminal,                   "ncmpcpp",   NULL,      NULL,      1 << 6,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "youtube-music-desktop-app", NULL,       NULL,      NULL,      1 << 6,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "zoom",                      NULL,       NULL,      NULL,      1 << 7,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "KeePassXC",                 NULL,       NULL,      NULL,      1 << 8,      -1,       1,         0,           1,           50,50,500,500,     5 },
	{ "qBittorrent",               NULL,       NULL,      NULL,      1 << 8,      -1,       1,         0,           1,           50,50,500,500,     5 },

	// floating rules
	{ "qimgv",                     NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,1450,900,      5 },
	{ "Hachoir-metadata-gtk",      NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,500,500,       5 },
	{ "Gcolor3",                   NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,800,400,       5 },
	{ "Gnome-calculator",          NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,500,700,       5 },
	{ "Android Emulator",          NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,500,500,       5 },
	{ "Blueberry.py",              NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,500,500,       5 },
	{ "Zoom Group Chat",           NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,500,500,       5 },
	{ "XVkbd",                     NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,900,300,       5 },
	{ "Gsimplecal",                NULL,       NULL,      NULL,      0,           -1,       1,         1,           0,           -1,-1,300,200,     5 },
  { "Emacs",                     NULL,      "Ediff",   "\uf1b2",   0,           -1,       1,         1,           1,           -1,-1,180,60,      5 },
  { "Blueman-manager",           NULL,       NULL,      NULL,      0,           -1,       1,         1,           1,           0,0,900,600,       5 },

  // prohibit focus
  { "Nemo-preview-start",        NULL,       NULL,      NULL,      0,           -1,       0,         1,           0,           -1,-1,-1,-1,       5 },
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

static char dmenumon[2]       = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };

static const char *roficmd[]           = { "rofi", "-show", "combi", NULL };
static const char *roficmd_power[]     = { "rofi", "-show", "power-menu", "-modi", "power-menu:~/Scripts/rofi-power-menu.sh", NULL };
static const char *roficmd_settings[]  = { "rofi", "-show", "system-settings", "-modi", "system-settings:~/Scripts/rofi-settings-menu.sh", NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { terminal, "--title", scratchpadname, NULL };

static const char *cmd_files_terminal[]     = { terminal, "--name", "ranger", "-e", "ranger", NULL };
static const char *cmd_music_terminal[]     = { terminal, "--name", "ncmpcpp", "-e", "ncmpcpp", NULL };
static const char *cmd_files[]              = { "nemo", NULL };
static const char *cmd_browser[]            = { "firefox", NULL };
static const char *cmd_video_player[]       = { "mpv", "--player-operation-mode=pseudo-gui", NULL };
static const char *cmd_screengrab[]         = { "/home/kyukee/Scripts/teiler-alt.sh", NULL };
static const char *cmd_rofi_refresh_files[] = { "/home/kyukee/Scripts/fmenu-rofi.sh", "-f", NULL };
static const char *cmd_color_picker[]       = { "/home/kyukee/Scripts/xcolor_notif.sh", NULL };
static const char *cmd_text_editor[]        = { "/home/kyukee/Scripts/emacs-server.sh", NULL };
static const char *cmd_lock[]               = { "/home/kyukee/Scripts/screen_lock.sh", NULL };

static const char *brightness_up[]   = {"xbacklight", "-inc", "5%", NULL };
static const char *brightness_down[] = {"xbacklight", "-dec", "5%", NULL };
static const char *volume_up[]       = {"pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *volume_down[]     = {"pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *volume_toggle[]   = {"pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_space,  spawn,          {.v = roficmd } },
	{ MODKEY|ControlMask,           XK_space,  spawn,          {.v = cmd_rofi_refresh_files } },
	{ MODKEY,                       XK_p,      spawn,          {.v = roficmd_power } },
	{ MODKEY,                       XK_s,      spawn,          {.v = roficmd_settings } },
	{ MODKEY,                       XK_e,      spawn,          {.v = cmd_files } },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = cmd_files_terminal } },
	{ MODKEY,                       XK_n,      spawn,          {.v = cmd_music_terminal } },
	{ MODKEY,                       XK_b,      spawn,          {.v = cmd_browser } },
	{ MODKEY,                       XK_u,      spawn,          {.v = cmd_text_editor } },
	{ MODKEY|ControlMask,           XK_l,      spawn,          {.v = cmd_lock } },
	{ MODKEY|ShiftMask,             XK_c,      spawn,          {.v = cmd_color_picker } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = cmd_screengrab } },
	{ MODKEY|ControlMask,           XK_m,      spawn,          {.v = cmd_video_player } },
	{ MODKEY|ControlMask,           XK_s,      spawn,          SHCMD("transset-df -a --dec .1") },
	{ MODKEY|ControlMask,           XK_d,      spawn,          SHCMD("transset-df -a --inc .1") },
	{ MODKEY|ControlMask,           XK_f,      spawn,          SHCMD("transset-df -a .75") },
	{ MODKEY|ControlMask,           XK_Return, togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_t,      tabmode,        {-1} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_plus,   incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
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
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_Left,   cyclelayout,    {.i = -1 } },
	{ MODKEY,                       XK_Right,  cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	// { MODKEY,                       XK_Tab,    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_7,      setborderpx,    {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      setborderpx,    {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_9,      setborderpx,    {.i = 0 } },
	{ MODKEY|ControlMask,           XK_e,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} },
	{ MODKEY,                       XK_o,      winview,        {0} },
  { 0,           XF86XK_AudioRaiseVolume,    spawn,          {.v = volume_up } },
  { 0,           XF86XK_AudioLowerVolume,    spawn,          {.v = volume_down } },
  { 0,           XF86XK_AudioMute,           spawn,          {.v = volume_toggle } },
  { 0,           XF86XK_MonBrightnessUp,     spawn,          {.v = brightness_up } },
  { 0,           XF86XK_MonBrightnessDown,   spawn,          {.v = brightness_down } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
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
