/* GIMP RGBA C-Source image dump (edit_paste.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[32 * 32 * 4 + 1];
} edit_paste = {
  32, 32, 4,
  "\377<\322\347\377<\322\346\377<\322\345\377<\322\344\377<\322\343\377<\322"
  "\342\377<\322\341\377;\322\237\377U\252\3\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377<\322\4\377<\322\243\377<\322\341\377<\322\342\377<\322"
  "\343\377<\322\344\377<\322\345\377<\322\346\377<\322\347\377<\322\376\377"
  "<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\277"
  "\3773\314\12\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377<\322\13\377<\322\301\377<\322\377\377<\322\377\377<"
  "\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\375\377<\322\377"
  "\377<\322\377\377<\322\377\377<\322\377\377;\322\301\377F\321\13\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377<\322\13\377<\322\300\377<\322\377\377<\322\377"
  "\377<\322\377\377<\322\377\377<\322\377\377<\322\374\377<\322\377\377<\322"
  "\377\377<\322\377\377<\322\302\377F\321\13\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377<\322\12\377<\322\276\377<\322\377\377<\322"
  "\377\377<\322\377\377<\322\377\377<\322\373\377<\322\377\377<\322\377\377"
  "<\321\304\377@\325\14\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\0\0\0\4\0\0\0\20\0\0\0\20\0\0\0\20\0\0\0\20"
  "\0\0\0\20\0\0\0\20\0\0\0\20\0\0\0\16\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377<\322\12"
  "\377<\322\275\377<\322\377\377<\322\377\377<\322\377\377<\322\372\377<\322"
  "\377\377<\322\305\377;\330\15\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0Q\0\0\0\310\0\0\0\363\0\0\0\360\0\0\0\360\0"
  "\0\0\360\0\0\0\360\0\0\0\360\0\0\0\360\0\0\0\360\0\0\0\362\0\0\0\350\0\0"
  "\0\250\0\0\0\35\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377<\322\11\377<\322\273\377<\322\377\377<\322\377\377<\322\371"
  "\377=\322\306\377;\330\15\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\0\0\0z\0\0\0\332\0\0\0G\0\0\0\11\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\0\0\0\30\0\0\0z\0\0\0\353\0\0\0(\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377<\322\11\377<\322\272\377"
  "<\322\377\377<\322\300\3777\333\16\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\0\0\0+\0\0\0\345\0\0\0\25\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\0\0\0j\0\0\0\304\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377<\322\10\377<\322\270\377@\325\14\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\0\0\0w\0\0\0\225\377\377\377\0\0\0\0:\0\15\11\333\0\21\13\337\0\21\13"
  "\337\0\21\13\337\0\21\13\337\0\21\13\337\0\21\13\337\0\21\13\337\0\21\13"
  "\337\0\17\12\335\0\0\0M\0\0\0\1\0\0\0\360\0\0\0\20\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377<\322\10"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0\235\0\0\0f\377\377\377\0\0\0\0H\0/!\362\2"
  "\317\217\377\2\317\217\377\2\317\217\377\2\317\217\377\2\317\217\377\2\317"
  "\217\377\2\317\217\377\2\317\217\377\4G2\373\26\26\26s\377\377\377\0\0\0"
  "\0\313\0\0\0""5\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\240"
  "\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2\377\260\377\2\377\260\377\2\377\260"
  "\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377"
  "\7U<\377,,,\215\377\377\377\0\0\0\0\310\0\0\0""8\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0\240\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2"
  "\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377"
  "\260\377\2\377\260\377\2\377\260\377\7S;\377777\235\377\377\377\0\0\0\0\310"
  "\0\0\0""8\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\240\0\0"
  "\0`\377\377\377\0\0\0\0H\1<(\360\2\377\260\377\2\377\260\377\2\377\260\377"
  "\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\10"
  "P9\377@@@\254\377\377\377\0\0\0\0\310\0\0\0""8\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0\240\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2"
  "\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377"
  "\260\377\2\377\260\377\2\377\260\377\10M7\377GGG\275\377\377\377\0\0\0\0"
  "\310\0\0\0""8\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\240"
  "\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2\377\260\377\2\377\260\377\2\377\260"
  "\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377"
  "\10K6\377MMM\317\377\377\377\0\0\0\0\310\0\0\0""8\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0\240\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2"
  "\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377"
  "\260\377\2\377\260\377\2\377\260\377\10G3\377RRR\341\377\377\377\0\0\0\0"
  "\310\0\0\0""8\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\240"
  "\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2\377\260\377\2\377\260\377\2\377\260"
  "\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377"
  "\10E2\377WWW\362\377\377\377\0\0\0\0\310\0\0\0""8\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0\240\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2"
  "\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\377"
  "\260\377\2\377\260\377\2\377\260\377\10B/\377ZZZ\377qqq\11\0\0\0\310\0\0"
  "\0""8\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\240\0\0\0"
  "`\377\377\377\0\0\0\0H\1<(\360\2\377\260\377\2\377\260\377\2\377\260\377"
  "\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\2\362\247\377\6"
  "&\34\377[[[\377|||#\0\0\0\310\0\0\0""8\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\0\0\0\240\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2\377\260\377"
  "\2\377\260\377\2\377\260\377\2\377\260\377\2\355\244\377\1\225g\377\7[A\377"
  "\5\30\22\377\25\30\27\377www\377\200\200\200>\0\0\0\310\0\0\0""8\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\240\0\0\0`\377\377\377\0"
  "\0\0\0H\1<(\360\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\1"
  "\243q\377\23\33\31\377\31\31\31\377\23\24\23\377ttt\377\177\177\177\377\200"
  "\200\200X\0\0\0\310\0\0\0""8\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\0\0\0\240\0\0\0`\377\377\377\0\0\0\0H\1<(\360\2\377\260\377\2\377\260"
  "\377\2\377\260\377\2\377\260\377\4uR\377\23\24\23\377\20\21\21\377rrr\377"
  "\177\177\177\377\177\177\177\333{{{\33\0\0\0\310\0\0\0""8\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\0\0\0\240\0\0\0`\377\377\377\0\0\0\0H\1<("
  "\360\2\377\260\377\2\377\260\377\2\377\260\377\2\377\260\377\7""3%\377\15"
  "\17\16\377ooo\377\177\177\177\377\177\177\177\331\200\200\200\32\377\377"
  "\377\0\0\0\0\310\0\0\0""8\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\0\0\0\216\0\0\0{\377\377\377\0\0\0\0H\0\3\2\374\5\26\21\377\6\34\25\377"
  "\6\40\30\377\7%\34\377\10\20\15\377mmm\377\177\177\177\377\200\200\200\330"
  "zzz\31\377\377\377\0\377\377\377\0\0\0\0\334\0\0\0&\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377<\322\200\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0X\0\0\0\301\377\377\377\0\0\0\0\6\0\0\0\31"
  "mmm\316jjj\320hhh\320eee\321rrr\315\200\200\200\310\177\177\177\271zzz\31"
  "\377\377\377\0\377\377\377\0\0\0\0#\0\0\0\357\0\0\0\1\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377<\322h"
  "\377<\322\377\377<\322\201\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\0\0\0\4\0\0\0\314\0\0\0s\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0\0\0\21\0\0\0\306"
  "\0\0\0q\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377<\322g\377<\322\376\377<\322\377\377<\322\377\377<\322"
  "\202\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\0\0\0!\0\0\0\306\0\0\0\316\0\0\0\216\0\0\0x\0\0\0x\0\0\0x\0\0\0x\0\0\0"
  "x\0\0\0x\0\0\0x\0\0\0z\0\0\0\243\0\0\0\345\0\0\0\207\0\0\0\2\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377<\322f\377"
  "<\322\376\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\203"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\0"
  "\0\0\2\0\0\0G\0\0\0z\0\0\0\210\0\0\0\210\0\0\0\210\0\0\0\210\0\0\0\210\0"
  "\0\0\210\0\0\0\210\0\0\0\207\0\0\0h\0\0\0'\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377<\322f\377<\322\376"
  "\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322"
  "\377\377<\322\204\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377<\322e\377<\322\376\377<\322\377\377<\322"
  "\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377"
  "<\322\377\377<\322\205\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377<\322d\377<\322\376\377<\322\377\377<\322\377\377<\322\377"
  "\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322"
  "\377\377<\322\377\377<\322\206\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377"
  "\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377<\322c"
  "\377<\322\376\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322"
  "\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377"
  "<\322\377\377<\322\377\377<\322\207\377\377\377\0\377\377\377\0\377\377\377"
  "\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0"
  "\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377\377\377\0\377"
  "\377\377\0\377\377\377\0\377\377\377\0\377<\322b\377<\322\376\377<\322\377"
  "\377<\322\377\377<\322\377\377<\322\377\377<\322\377\377<\322\377",
};

