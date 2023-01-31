/* See LICENSE file for copyright and license details. */
#include "gaplessgrid.c"
#include "movestack.c"

#define PROJECT_PATH ~/Projects/Delayless
#define STR1(PATH) #PATH
#define STR(PATH) STR1(PATH)

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 0;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "Source Han Serif CN:size=9",
										"FiraCode Nerd Font:size=10:antialias=true:autohint=true",
										"Monospace:size=8:antialias=true:autohint=true"};
static const int statusfontindex =1; //Select font of font list to use for statusbar
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#384B54";
static const char col_border[]      = "#2F98ED";
// gruvbox
static const char col_gb_bg[]       = "#282828";
static const char col_gb_fg[]       = "#ebdbb2";

static const char col_gb_red1[]     = "#cc241d";
static const char col_gb_red2[]     = "#fb4934";

static const char col_gb_green1[]   = "#98971a";
static const char col_gb_green2[]   = "#b8bb26";

static const char col_gb_yellow1[]  = "#d79921";
static const char col_gb_yellow2[]  = "#fabd2f";

static const char col_gb_blue1[]    = "#458588";
static const char col_gb_blue2[]    = "#83a598";

static const char col_gb_purple1[]  = "#b16286";
static const char col_gb_purple2[]  = "#83869b";

static const char col_gb_aqua1[]    = "#689d6a";
static const char col_gb_aqua2[]    = "#8ec07c";

static const char col_gb_gray1[]    = "#a89984";
static const char col_gb_gray2[]    = "#928374";

static const char col_gb_orange1[]  = "#d65d0e";
static const char col_gb_orange2[]  = "#fe8019";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
// gruvbox
static const char *colors[][3]      = {
	/*                      fg                 bg          border   */
	/* [SchemeHid]      = { col_cyan,         col_gray1,    col_border}, */
	[SchemeNorm]     = { col_gb_gray2,     col_gb_bg,    col_gb_bg },
	[SchemeSel]      = { col_gb_orange2,   col_gb_bg,    col_gb_orange2 },
	[SchemeSel2]     = { col_gb_fg,        col_gb_bg,    col_gb_bg },
	[SchemeUrgent]   = { col_gb_red2,      col_gb_bg,    col_gb_bg },
	[SchemeTitle]    = { col_gb_fg,        col_gb_bg,    col_gb_bg },
	[SchemeLt]       = { col_gb_orange2,   col_gb_bg,    col_gb_bg },
	[SchemeNotify]   = { col_gb_red1,      col_gb_bg,    col_gb_bg },
	[SchemeStatus]   = { col_gb_orange1,   col_gb_bg,    col_gb_bg },
	[SchemeIndOff]   = { col_gb_green2,    col_gb_bg,    col_gb_bg },
	[SchemeIndOn]    = { col_gb_blue2,     col_gb_bg,    col_gb_bg },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm]     = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]      = { OPAQUE, baralpha, borderalpha },
	[SchemeSel2]     = { OPAQUE, baralpha, borderalpha },
	[SchemeUrgent]   = { OPAQUE, baralpha, borderalpha },
	[SchemeTitle]    = { OPAQUE, baralpha, borderalpha },
	[SchemeLt]       = { OPAQUE, baralpha, borderalpha },
	[SchemeNotify]   = { OPAQUE, baralpha, borderalpha },
	[SchemeStatus]   = { OPAQUE, baralpha, borderalpha },
	[SchemeIndOff]   = { OPAQUE, baralpha, borderalpha },
	[SchemeIndOn]    = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
/*        嗢 */
static const char *tags[] = { "\uf489", "\ue62b", "\uf7a3", "\uf268", "\uf886", "\uf198", "\uf1c1", "\ufa7b", "\uf019" };
 /* ❶ ❷ ❸ ❹ ❺ ❻ ❼ ❽ ❾ */
/* static const char *tagsalt[] = { "\u2776", "\u2777", "\u2778", "\u2779", "\u277a", "\u277b", "\u277c", "\u277d", "\u277e" }; */
/*          */
static const char *tagsalt[] = { "\uf8a3", "\uf8a6", "\uf8a9", "\uf8ac", "\uf8af", "\uf8b2", "\uf8b5", "\uf8b8", "\uf8bb" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                   instance  title           tags mask  isfloating  isalwaysontop  isterminal  noswallow  monitor */
	{ "Gimp",                  NULL,     NULL,           0,         0,          0,             0,           0,        -1 },
	{ "Google-chrome",         NULL,     NULL,           1 << 3,    0,          0,             0,          -1,        -1 },
	{ "st-256color",           NULL,     NULL,           0,         0,          0,             1,           0,        -1 },
	{ "KeePassXC",             NULL,     NULL,           1 << 5,    0,          0,             0,           0,        -1 },
	{ "zathura",               NULL,     NULL,           1 << 6,    0,          0,             0,           0,        -1 },
	{ NULL,                    NULL, "Listen1",          0,         1,          1,             0,           1,        -1 },
	{ NULL,                    NULL, "dragon",           0,         1,          1,             0,           1,        -1 },
	{ "listen1",               NULL,     NULL,           1 << 4,    1,          0,             0,           0,        -1 },
	{ "netease-cloud-music",   NULL,     NULL,           1 << 4,    1,          1,             0,           0,        -1 },
	{ "fluent-reader",         NULL,     NULL,           1 << 6,    1,          0,             0,           0,        -1 },
	{ "Slack",                 NULL,     NULL,           1 << 5,    0,          0,             0,           0,        -1 },
    { "float",                 NULL,     NULL,           0,         1,          0,             0,           0,        -1 },
	{  NULL,                   NULL,     "mutt",         1 << 5,    0,          0,             0,           0,        -1 },
	{ "rofi",                  NULL,     NULL,           0,         0,          1,             0,           0,        -1 },
	{ NULL,                    NULL,     "Event Tester", 0,         0,          0,             0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	/*      﩯 響☰ */
	{ "\uf04c",    tile },    /* first entry is default */
	{ "\uf2d2",    NULL },    /* no layout function means floating behavior */
	{ "\uf2d0",    monocle },
	{ "\ufa6f",    gaplessgrid },
	{ "\ufa69",    bstack },
	{ "\u2630",    bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define HOLDKEY 0xffeb // 0 - disable; 0xffe9 - Mod1Mask; 0xffeb - Mod4Mask
#define HOLDKEY_R 0xffec

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, topbar ? NULL : "-b", NULL };
static const char *termcmd[]  = { "st", NULL };
/* static const char *mailcmd[] = { "urxvtc", "-title", "mutt", "-e", "mutt", NULL }; */
static const char *browsercmd[]  = { "google-chrome-stable", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "138x40", NULL };
static const char *screenshotcmd[] = { "flameshot", "gui", NULL };
static const char *rofirun[] = { "rofi", "-show", "drun", NULL };
static const char *rofiwindow[] = { "rofi", "-show", "combi", NULL };

#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = rofirun } },
	{ MODKEY,                       XK_w,      spawn,          {.v = rofiwindow } },
	/* { 0,      XF86XK_AudioLowerVolume,      spawn,          SHCMD("amixer set Master 5%-") }, */
	{ MODKEY|ControlMask|ShiftMask, XK_F12,    spawn,          SHCMD("reboot") },
	{ MODKEY|ControlMask|ShiftMask, XK_r,      spawn,          SHCMD("reboot") },
	/* { 0,      XF86XK_AudioRaiseVolume,      spawn,          SHCMD("amixer set Master 5%+") }, */
	/* { 0,             XF86XK_AudioMute,      spawn,          SHCMD("amixer set Master toggle") }, */
	{ 0,                XF86XK_AudioMute,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/volume_toggle.sh --toggle") },
	{ 0,         XF86XK_AudioLowerVolume,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/volume_down.sh") },
	{ 0,         XF86XK_AudioRaiseVolume,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/volume_up.sh") },
	{ MODKEY,                      XK_F1,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/volume_toggle.sh --toggle") },
	{ MODKEY,                      XK_F2,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/volume_down.sh") },
	{ MODKEY,                      XK_F3,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/volume_up.sh") },
	{ MODKEY,                      XK_F5,      spawn,          SHCMD("brightnessctl set 10%-") },
	{ MODKEY,                      XK_F6,      spawn,          SHCMD("brightnessctl set +10%") },
	/* { MODKEY,					XK_F5,      spawn,          SHCMD("xrandr --output eDP-1 --brightness 0.2") }, */
	/* { MODKEY,					XK_F6,      spawn,          SHCMD("xrandr --output eDP-1 --brightness 0.8") }, */
	{ 0,        XF86XK_MonBrightnessDown,      spawn,          SHCMD("brightnessctl set 10%-") },
	{ 0,          XF86XK_MonBrightnessUp,      spawn,          SHCMD("brightnessctl set +10%") },
	// Fn+F7 toggle airplane mode
	{ 0,                   XF86XK_RFKill,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/network_toggle.sh") },
	{ MODKEY,                      XK_F8,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/network_toggle.sh") },
	/* { MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("rofi -show combi") }, */
	{ MODKEY,                       XK_v,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/greenclipboard.sh") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("pkill picom; sleep 0.5; picom -b --experimental-backends --config ~/.config/picom.conf") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/wallpaper-change.sh") },
	{ MODKEY,                       XK_y,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/qwerty.sh") },
	{ MODKEY|ShiftMask,             XK_y,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/colemak.sh") },
	{ MODKEY|ShiftMask,            XK_F4,      spawn,          SHCMD(STR(PROJECT_PATH)"/scripts/suspend.sh") },
    { MODKEY,                  XK_Return,      spawn,          {.v = termcmd } },
    { MODKEY|ControlMask,      XK_Return,      spawn,          SHCMD("st -c float") },
    { MODKEY,                       XK_c,      spawn,          {.v = browsercmd } },
    /* { MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("netease-cloud-music") }, */
    { MODKEY,                       XK_m,      spawn,          SHCMD("listen1") },
    { MODKEY|ControlMask,           XK_l,      spawn,          SHCMD("xtrlock") },
    { 0,                        XK_Print,      spawn,          {.v = screenshotcmd } },
    { MODKEY,              XK_apostrophe,      togglescratch,  {.v = scratchpadcmd } },
	{ 0,                         HOLDKEY,      holdbar,        {0} },
	{ 0,                       HOLDKEY_R,      holdbar,        {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_r,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_r,      rotatestack,    {.i = -1 } },
	{ MODKEY|ShiftMask,         XK_minus,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,         XK_equal,      incnmaster,     {.i = -1 } },
	{ MODKEY,                   XK_minus,      setmfact,       {.f = -0.05} },
	{ MODKEY,                   XK_equal,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,        XK_Return,      zoom,           {0} },
	{ MODKEY|ControlMask|ShiftMask,XK_Return,  savelog,        {0} },
	{ MODKEY,                     XK_Tab,      view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_u,      focusurgent,    {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ControlMask,           XK_t,      setlayout,      {.v = &layouts[5]} },
	/* { MODKEY|ControlMask,       XK_space,      setlayout,      {0} }, */
	{ MODKEY|ShiftMask,         XK_space,      togglefloating, {0} },
	{ MODKEY,                   XK_space,      focusmaster,    {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_s,      toggleskipping, {0} },
	{ MODKEY,                       XK_s,      togglealwaysontop, {0} },
	{ MODKEY,                       XK_h,      viewtoleft,     {0} },
	{ MODKEY,                       XK_l,      viewtoright,    {0} },
	{ MODKEY|ShiftMask,             XK_h,      tagtoleft,      {0} },
	{ MODKEY|ShiftMask,             XK_l,      tagtoright,     {0} },
	{ MODKEY,                       XK_x,      transfer,       {0} },
	{ MODKEY,                  XK_period,      focusmon,       {.i = -1 } },
	{ MODKEY,                   XK_comma,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,        XK_period,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,         XK_comma,      tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_e,      togglealttag,   {0} },
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
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

