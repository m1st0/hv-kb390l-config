/*
 *      Copyright 2018 Pavel Bludov <pbludov@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License along
 *      with this program; if not, write to the Free Software Foundation, Inc.,
 *      51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "usbscancodeedit.h"

#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof(*x))
#endif

// http://www.usb.org/developers/hidpage/Hut1_12v2.pdf (10 Keyboard/Keypad Page 0x07)
// https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/hid/hid-input.c (lines 39-55)

enum KeyGroup
{
    RESERVED,
    LETTER,
    DIGIT,
    CONTROL,
    SYMBOLS,
    FUNCTIONAL,
    NAVIGATION,
    TOGGLE,
    KEYPAD_MAIN,
    KEYPAD_EXTRA,
    MODIFIERS,
    INTERNATIONAL,
    MEDIA,
    INTERNET,
    OTHER
};

static struct EnumEdit::Item items[] =
{
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"A", LETTER},
    {"B", LETTER},
    {"C", LETTER},
    {"D", LETTER},
    {"E", LETTER},
    {"F", LETTER},
    {"G", LETTER},
    {"H", LETTER},
    {"I", LETTER},
    {"J", LETTER},
    {"K", LETTER},
    {"L", LETTER},

    {"M", LETTER},
    {"N", LETTER},
    {"O", LETTER},
    {"P", LETTER},
    {"Q", LETTER},
    {"R", LETTER},
    {"S", LETTER},
    {"T", LETTER},
    {"U", LETTER},
    {"V", LETTER},
    {"W", LETTER},
    {"X", LETTER},
    {"Y", LETTER},
    {"Z", LETTER},
    {"1", DIGIT},
    {"2", DIGIT},

    {"3", DIGIT},
    {"4", DIGIT},
    {"5", DIGIT},
    {"6", DIGIT},
    {"7", DIGIT},
    {"8", DIGIT},
    {"9", DIGIT},
    {"0", DIGIT},
    {"ENTER", CONTROL},
    {"ESCAPE", CONTROL},
    {"BACKSPACE", CONTROL},
    {"TAB", CONTROL},
    {"SPACE", CONTROL},
    {"-", SYMBOLS},
    {"=", SYMBOLS},
    {"[", SYMBOLS},

    {"]", SYMBOLS},
    {"\\", SYMBOLS},
    {"~", SYMBOLS},
    {";", SYMBOLS},
    {"'", SYMBOLS},
    {"`", SYMBOLS},
    {",", SYMBOLS},
    {".", SYMBOLS},
    {"/", SYMBOLS},
    {"CAPSLOCK", TOGGLE},
    {"F1", FUNCTIONAL},
    {"F2", FUNCTIONAL},
    {"F3", FUNCTIONAL},
    {"F4", FUNCTIONAL},
    {"F5", FUNCTIONAL},
    {"F6", FUNCTIONAL},

    {"F7", FUNCTIONAL},
    {"F8", FUNCTIONAL},
    {"F9", FUNCTIONAL},
    {"F10", FUNCTIONAL},
    {"F11", FUNCTIONAL},
    {"F12", FUNCTIONAL},
    {"SYSRQ", CONTROL},
    {"SCROLLLOCK", TOGGLE},
    {"PAUSE", CONTROL},
    {"INSERT", CONTROL},
    {"HOME", NAVIGATION},
    {"PGUP", NAVIGATION},
    {"DELETE", CONTROL},
    {"END", NAVIGATION},
    {"PGDOWN", NAVIGATION},
    {"RIGHT", NAVIGATION},

    {"LEFT", NAVIGATION},
    {"DOWN", NAVIGATION},
    {"UP", NAVIGATION},
    {"NUMLOCK", TOGGLE},
    {"Keypad /", KEYPAD_MAIN},
    {"Keypad *", KEYPAD_MAIN},
    {"Keypad -", KEYPAD_MAIN},
    {"Keypad +", KEYPAD_MAIN},
    {"Keypad ENTER", KEYPAD_MAIN},
    {"Keypad 1", KEYPAD_MAIN},
    {"Keypad 2", KEYPAD_MAIN},
    {"Keypad 3", KEYPAD_MAIN},
    {"Keypad 4", KEYPAD_MAIN},
    {"Keypad 5", KEYPAD_MAIN},
    {"Keypad 6", KEYPAD_MAIN},
    {"Keypad 7", KEYPAD_MAIN},

    {"Keypad 8", KEYPAD_MAIN},
    {"Keypad 9", KEYPAD_MAIN},
    {"Keypad 0", KEYPAD_MAIN},
    {"Keypad DELETE", KEYPAD_MAIN},
    {"|", SYMBOLS},
    {"Compose", OTHER},
    {"Power", OTHER},
    {"Keypad =", KEYPAD_MAIN},
    {"F13", FUNCTIONAL},
    {"F14", FUNCTIONAL},
    {"F15", FUNCTIONAL},
    {"F16", FUNCTIONAL},
    {"F17", FUNCTIONAL},
    {"F18", FUNCTIONAL},
    {"F19", FUNCTIONAL},
    {"F20", FUNCTIONAL},

    {"F21", FUNCTIONAL},
    {"F22", FUNCTIONAL},
    {"F23", FUNCTIONAL},
    {"F24", FUNCTIONAL},
    {"Open", OTHER},
    {"Help", OTHER},
    {"Props", OTHER},
    {"Front", OTHER},
    {"Stop", OTHER},
    {"Again", OTHER},
    {"Undo", OTHER},
    {"Cut", OTHER},
    {"Copy", OTHER},
    {"Paste", OTHER},
    {"Find", OTHER},
    {"Mute", OTHER},

    {"Volume Up", OTHER},
    {"Volume Down", OTHER},
    {"Locking Caps Lock", TOGGLE},
    {"Locking Num Lock", TOGGLE},
    {"Locking Scroll Lock", TOGGLE},
    {"Keypad ,", KEYPAD_EXTRA},
    {"Keypad = (AS/400)", KEYPAD_EXTRA},
    {"International1", INTERNATIONAL},
    {"International2", INTERNATIONAL},
    {"International3", INTERNATIONAL},
    {"International4", INTERNATIONAL},
    {"International5", INTERNATIONAL},
    {"International6", INTERNATIONAL},
    {"International7", INTERNATIONAL},
    {"International8", INTERNATIONAL},
    {"International9", INTERNATIONAL},

    {"Hangul", INTERNATIONAL},
    {"Hangul_Hanja", INTERNATIONAL},
    {"Katakana", INTERNATIONAL},
    {"Hiragana", INTERNATIONAL},
    {"LANG5", INTERNATIONAL},
    {"LANG6", INTERNATIONAL},
    {"LANG7", INTERNATIONAL},
    {"LANG8", INTERNATIONAL},
    {"LANG9", INTERNATIONAL},
    {"Erase", OTHER},
    {"Attention", OTHER},
    {"Cancel", OTHER},
    {"Clear", OTHER},
    {"Prior", OTHER},
    {"Return", OTHER},
    {"Separator", OTHER},

    {"Out", OTHER},
    {"Oper", OTHER},
    {"Clear/Again", OTHER},
    {"CrSel/Props", OTHER},
    {"ExSel", OTHER},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},

    {"Keypad 00", KEYPAD_EXTRA},
    {"Keypad 000", KEYPAD_EXTRA},
    {"Keypad Thousands Separator", KEYPAD_EXTRA},
    {"Keypad Decimal Separator", KEYPAD_EXTRA},
    {"Keypad Currency Unit", KEYPAD_EXTRA},
    {"Keypad Currency Sub-unit", KEYPAD_EXTRA},
    {"Keypad (", KEYPAD_MAIN},
    {"Keypad )", KEYPAD_MAIN},
    {"Keypad {", KEYPAD_EXTRA},
    {"Keypad }", KEYPAD_EXTRA},
    {"Keypad Tab", KEYPAD_EXTRA},
    {"Keypad Backspace", KEYPAD_MAIN},
    {"Keypad A", KEYPAD_EXTRA},
    {"Keypad B", KEYPAD_EXTRA},
    {"Keypad C", KEYPAD_EXTRA},
    {"Keypad D", KEYPAD_EXTRA},
    {"Keypad E", KEYPAD_EXTRA},

    {"Keypad F", KEYPAD_EXTRA},
    {"Keypad XOR", KEYPAD_EXTRA},
    {"Keypad ^", KEYPAD_EXTRA},
    {"Keypad %", KEYPAD_EXTRA},
    {"Keypad <", KEYPAD_EXTRA},
    {"Keypad >", KEYPAD_EXTRA},
    {"Keypad &", KEYPAD_EXTRA},
    {"Keypad &&", KEYPAD_EXTRA},
    {"Keypad |", KEYPAD_EXTRA},
    {"Keypad ||", KEYPAD_EXTRA},
    {"Keypad :", KEYPAD_EXTRA},
    {"Keypad #", KEYPAD_EXTRA},
    {"Keypad Space", KEYPAD_EXTRA},
    {"Keypad @", KEYPAD_EXTRA},
    {"Keypad !", KEYPAD_EXTRA},
    {"Keypad Memory Store", KEYPAD_EXTRA},

    {"Keypad Memory Recall", KEYPAD_EXTRA},
    {"Keypad Memory Clear", KEYPAD_EXTRA},
    {"Keypad Memory Add", KEYPAD_EXTRA},
    {"Keypad Memory Subtract", KEYPAD_EXTRA},
    {"Keypad Memory Multiply", KEYPAD_EXTRA},
    {"Keypad Memory Divide", KEYPAD_EXTRA},
    {"Keypad +/-", KEYPAD_EXTRA},
    {"Keypad Clear", KEYPAD_EXTRA},
    {"Keypad Clear Entry", KEYPAD_EXTRA},
    {"Keypad Binary", KEYPAD_EXTRA},
    {"Keypad Octal", KEYPAD_EXTRA},
    {"Keypad Decimal", KEYPAD_EXTRA},
    {"Keypad Hexadecimal", KEYPAD_EXTRA},
    {"", RESERVED},
    {"", RESERVED},

    {"Left Control", MODIFIERS},
    {"Left Shift", MODIFIERS},
    {"Left Alt", MODIFIERS},
    {"Left Super", MODIFIERS},
    {"Right Control", MODIFIERS},
    {"Right Shift", MODIFIERS},
    {"Right Alt", MODIFIERS},
    {"Right Super", MODIFIERS},
    {"Audio Play", MEDIA},
    {"Audio Stop", MEDIA},
    {"Audio Prev", MEDIA},
    {"Audio Next", MEDIA},
    {"Eject", MEDIA},
    {"Audio Volume Up", MEDIA},
    {"Audio Volume Down", MEDIA},
    {"Audio Mute", MEDIA},

    {"WWW Browser", INTERNET},
    {"WWW Back", INTERNET},
    {"WWW Forward", INTERNET},
    {"WWW Stop", INTERNET},
    {"WWW Search", INTERNET},
    {"Scroll Up", INTERNET},
    {"Scroll Down", INTERNET},
    {"", RESERVED},
    {"Sleep", OTHER},
    {"Screen Saver", OTHER},
    {"WWW Reload", INTERNET},
    {"Calculator", OTHER},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
    {"", RESERVED},
},
groups[] =
{
    {QT_TR_NOOP("Letter"),         LETTER},
    {QT_TR_NOOP("Digit"),          DIGIT},
    {QT_TR_NOOP("Control"),        CONTROL},
    {QT_TR_NOOP("Symbols"),        SYMBOLS},
    {QT_TR_NOOP("Functional"),     FUNCTIONAL},
    {QT_TR_NOOP("Navigation"),     NAVIGATION},
    {QT_TR_NOOP("Toggle"),         TOGGLE},
    {QT_TR_NOOP("Keypad"),         KEYPAD_MAIN},
    {QT_TR_NOOP("Keypad (extra)"), KEYPAD_EXTRA},
    {QT_TR_NOOP("Modifiers"),      MODIFIERS},
    {QT_TR_NOOP("International"),  INTERNATIONAL},
    {QT_TR_NOOP("Media"),          MEDIA},
    {QT_TR_NOOP("Internet"),       INTERNET},
    {QT_TR_NOOP("Other"),          OTHER},
};

static std::vector<EnumEdit::Item> usbKeyItems(items, items + _countof(items));
static std::vector<EnumEdit::Item> usbKeyGroups(groups, groups + _countof(groups));

UsbScanCodeEdit::UsbScanCodeEdit(QWidget *parent)
    : EnumEdit(usbKeyItems, usbKeyGroups, parent)
{
}

