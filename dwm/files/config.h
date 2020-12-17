/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 3;                 /* gaps between windows */
static const int showbar            = 2;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
//static const char *fonts[]          = { "monospace:size=12" };
static const char *fonts[]          = { "FiraCode Nerd Font:style=Retina,Regular:size=12" , "monospace:size=18" };
//static const char dmenufont[]       = "monospace:size=12";
static const char dmenufont[]       = "FiraCode Nerd Font:style=Retina,Regular:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor    scratch key */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1,        0  },
	{ NULL,       NULL,   "scratchpad",   0,            1,           -1,       's' },
};

/* layout(s) */
static const float mfact     = 0.75; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define HOLDKEY 0xffe9 // replace 0 with the keysym to activate holdbar

#include "ru_gaps.c"
#include "ru_centeredmaster.c"
#include "ru_bottomstack.c"
#include "ru_gaplessgrid.c"
#include "ru_deck.c"
#include "setborderpx.c"
#include "switchcol.c"
#include "focusurgent.c"
#include "movestack.c"
#include "namedscratchpads.c"
#include "zoomswap.c"
#include "moveresize.c"
#include "moveplace.c"
/* #include "transfer.c" */

/* static const char *statusresetcmd[]  = { "xsetroot", "-name", "axpira", NULL}; */
#include "holdbar.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[]~",      tiled },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "###",      gaplessgrid },
	{ "[D]",      deck },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
// static const char *termcmd[]  = { "alacritty", NULL };
/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", "st", "-t", "scratchpad", NULL}; 

/* static const char *statuscmd[]  = { "xsetroot", "-name", "\"$(status.sh)\"", NULL}; */
static const char *statuscmd[]  = { "status.sh", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ControlMask|Mod4Mask,              XK_j,      moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY|ControlMask|Mod4Mask,              XK_k,      moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY|ControlMask|Mod4Mask,              XK_l,      moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY|ControlMask|Mod4Mask,              XK_h,      moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ControlMask|Mod4Mask|ShiftMask,    XK_j,      moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ControlMask|Mod4Mask|ShiftMask,    XK_k,      moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ControlMask|Mod4Mask|ShiftMask,    XK_l,      moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ControlMask|Mod4Mask|ShiftMask,    XK_h,      moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|Mod4Mask,              XK_j,      moveresize,     {.v = "0x 100y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_k,      moveresize,     {.v = "0x -100y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_l,      moveresize,     {.v = "100x 0y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_h,      moveresize,     {.v = "-100x 0y 0w 0h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_j,      moveresize,     {.v = "0x 0y 0w 100h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_k,      moveresize,     {.v = "0x 0y 0w -100h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_l,      moveresize,     {.v = "0x 0y 100w 0h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_h,      moveresize,     {.v = "0x 0y -100w 0h" } },
	{ MODKEY|Mod5Mask,              XK_q,      moveplace,      {.ui = WIN_NW }},
	{ MODKEY|Mod5Mask,              XK_w,      moveplace,      {.ui = WIN_N  }},
	{ MODKEY|Mod5Mask,              XK_e,      moveplace,      {.ui = WIN_NE }},
	{ MODKEY|Mod5Mask,              XK_a,      moveplace,      {.ui = WIN_W  }},
	{ MODKEY|Mod5Mask,              XK_s,      moveplace,      {.ui = WIN_C  }},
	{ MODKEY|Mod5Mask,              XK_d,      moveplace,      {.ui = WIN_E  }},
	{ MODKEY|Mod5Mask,              XK_z,      moveplace,      {.ui = WIN_SW }},
	{ MODKEY|Mod5Mask,              XK_x,      moveplace,      {.ui = WIN_S  }},
	{ MODKEY|Mod5Mask,              XK_c,      moveplace,      {.ui = WIN_SE }},
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|Mod4Mask,              XK_0,      setgaps,        {.i = 0  } },
	{ MODKEY|ShiftMask,             XK_minus,  setborderpx,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setborderpx,    {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,      setborderpx,    {.i = 0 } },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask|ControlMask, XK_h,      setcfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask|ControlMask, XK_l,      setcfact,       {.f = -0.01} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY|ControlMask,           XK_l,      setmfact,       {.f = +0.01} },
	/* { MODKEY,                       XK_n,      switchcol,      {0} }, */
	{ MODKEY|ShiftMask,             XK_n,      focusurgent,    {0} },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	/* { MODKEY,                       XK_x,      zoomswap,       {0} }, */
	/* { MODKEY,                       XK_Return, zoom,           {0} }, */
	{ MODKEY,                       XK_Return, zoomswap,           {0} },
	{ MODKEY|ControlMask,           XK_Return, switchcol,           {0} },
	/* { MODKEY|ControlMask,           XK_Return, transfer,           {0} }, */
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
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
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ 0,                            HOLDKEY,   holdbar,        {.v = statuscmd} },
	/* { 0,                            HOLDKEY,   holdbar,        {0} }, */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

