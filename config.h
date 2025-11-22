/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include <X11/keysym.h>

/*

Kolin63's dwm build

patches applied:
dwm-statuscolors-20220322-bece862.diff
dwm-vanitygaps-20190508-6.2.diff
dwm-actualfullscreen-20211013-cb3f58a.diff
dwm-pertag-6.2.diff

*/

// clang-format off

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=10" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_black[]       = "#000000";
static const char col_red[]         = "#ff0000";
static const char col_yellow[]      = "#ffff00";
static const char col_white[]       = "#ffffff";

static const char *colors[][3]      = {
  /*                            fg          bg          border   */
  /* SchemeNorm   \x01   */   { col_gray3,  col_gray1,  col_gray2 },
  /* SchemeSel    \x02   */   { col_gray4,  col_cyan,   col_cyan  },
  /* gray         \x03   */   { "#999999",  col_gray1,  col_gray1 },
  /* cyan         \x04   */   { col_cyan,   col_gray1,  col_gray1 },
  /* black        \x05   */   { col_black,  col_gray1,  col_gray1 },
  /* red          \x06   */   { col_red,    col_gray1,  col_gray1 },
  /* yellow       \x07   */   { col_yellow, col_gray1,  col_gray1 },
  /* white        \x08   */   { col_white,  col_gray1,  col_gray1 },
  /* spotify      \x09   */   { "#1DB954",  col_gray1,  col_gray1 },
  /* gruvboxblue  \x0a   */   { "#458588",  col_gray1,  col_gray1 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
   */
  /* class                            instance    title       tags mask     isfloating   monitor */
  { "qutebrowser",                    NULL,       NULL,       1 << 1,       0,           -1 },
  { "firefox_firefox",                NULL,       NULL,       1 << 1,       0,           -1 },
  { "FreeCAD",                        NULL,       NULL,       1 << 2,       0,           -1 },
  { "Blockbench",                     NULL,       NULL,       1 << 2,       0,           -1 },
  { "BambuStudio",                    NULL,       NULL,       1 << 2,       0,           -1 },
  { "Audacity",                       NULL,       NULL,       1 << 2,       0,           -1 },
  { "BoltLauncher",                   NULL,       NULL,       1 << 3,       0,           -1 },
  { "PrismLauncher",                  NULL,       NULL,       1 << 3,       0,           -1 },
  { "net-runelite-launcher-Launcher", NULL,       NULL,       1 << 3,       1,           -1 },
  { "net-runelite-client-RuneLite",   NULL,       NULL,       1 << 3,       1,           -1 },
  { "steam",                          NULL,       NULL,       1 << 3,       0,           -1 },
  { "Lutris",                         NULL,       NULL,       1 << 3,       0,           -1 },
  { "CurseForge",                     NULL,       NULL,       1 << 3,       0,           -1 },
  { "discord",                        NULL,       NULL,       1 << 7,       0,           -1 },
  { "Slack",                          NULL,       NULL,       1 << 7,       0,           -1 },
  { "Spotify",                        NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };

static const char *volumeupcmd[]   = { "amixer", "-D", "pulse", "set", "Master", "5%+", NULL };
static const char *volumedowncmd[] = { "amixer", "-D", "pulse", "set", "Master", "5%-", NULL };
static const char *volumemutecmd[] = { "amixer", "-D", "pulse", "set", "Master", "0%", NULL };

static const char *musicplay[]     = { "dbus-send", "--print-reply", "--dest=org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.PlayPause", NULL };
static const char *musicstop[]     = { "dbus-send", "--print-reply", "--dest=org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.Stop", NULL };
static const char *musicprev[]     = { "dbus-send", "--print-reply", "--dest=org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.Previous", NULL };
static const char *musicnext[]     = { "dbus-send", "--print-reply", "--dest=org.mpris.MediaPlayer2.spotify", "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player.Next", NULL };

static const char *screenshotwhole[]       = { "sh", "-c", "gnome-screenshot -c -f \"$HOME/Pictures/Screenshots/$(date +%Y-%m-%d-%H%M%S).png\"", NULL };
static const char *screenshotarea[]        = { "sh", "-c", "gnome-screenshot -c -a -f \"$HOME/Pictures/Screenshots/$(date +%Y-%m-%d-%H%M%S).png\"", NULL };
static const char *screenshotinteractive[] = { "sh", "-c", "gnome-screenshot -c -i -f \"$HOME/Pictures/Screenshots/$(date +%Y-%m-%d-%H%M%S).png\"", NULL };

static const Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
  { MODKEY,                       XK_Return, spawn,          {.v = (const char*[]){ "alacritty", NULL } } },
  { MODKEY,                       XK_semicolon, spawn,       {.v = (const char*[]){ "qutebrowser", NULL } } },

  { MODKEY,                       XK_c,      spawn,          {.v = (const char*[]){ "mousetoggle", NULL } } },
  { MODKEY|ShiftMask,             XK_c,      spawn,          {.v = (const char*[]){ "ansiicolor", NULL } } },
  { MODKEY,                       XK_e,      spawn,          {.v = (const char*[]){ "emojiselect", NULL } } },

  { 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = volumeupcmd } },
  { 0,                            XF86XK_AudioLowerVolume, spawn, {.v = volumedowncmd } },
  { 0,                            XF86XK_AudioMute, spawn,   {.v = volumemutecmd } },

  // run visudo then add this line
  // colin   ALL=(ALL) NOPASSWD: /usr/bin/brightnessctl
  { 0,                            XF86XK_MonBrightnessUp, spawn, { .v = (const char*[]){ "sudo", "brightnessctl", "s", "+5%", NULL } } },
  { 0,                            XF86XK_MonBrightnessDown, spawn, { .v = (const char*[]){ "sudo", "brightnessctl", "s", "5%-", NULL } } },

  { 0,                            XF86XK_AudioPlay, spawn,   {.v = musicplay } },
  { 0,                            XF86XK_AudioStop, spawn,   {.v = musicstop } },
  { 0,                            XF86XK_AudioPrev, spawn,   {.v = musicprev } },
  { 0,                            XF86XK_AudioNext, spawn,   {.v = musicnext } },

  { 0,                            XK_Print, spawn,       {.v = screenshotwhole} },
  { ShiftMask,                    XK_Print, spawn,       {.v = screenshotarea} },
  { ControlMask,                  XK_Print, spawn,       {.v = screenshotinteractive} },

  { MODKEY,                       XK_b,      togglebar,      {0} },

  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

  { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },

  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },

	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	// { MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	// { MODKEY|Mod1Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
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

  { MODKEY,                       XK_z,      zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY|ShiftMask,             XK_q,      killclient,     {0} },

	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },

  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  TAGKEYS(                        XK_5,                      4)
  TAGKEYS(                        XK_6,                      5)
  TAGKEYS(                        XK_7,                      6)
  TAGKEYS(                        XK_8,                      7)
  TAGKEYS(                        XK_9,                      8)

  { MODKEY|ShiftMask,             XK_Escape, quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

