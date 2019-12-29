/* GIMP RGBA C-Source image dump (type_diag_state.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[32 * 24 * 4 + 1];
} type_diag_state = {
  32, 24, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377"
  "\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\377\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200@\177\177\177"
  "\365\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\365\200\200\200"
  "@\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\010\200\200\200n\200\200\200"
  "\250\177\177\177\273\177\177\177\273\177\177\177\273\177\177\177\257\200"
  "\200\200~\200\200\200\026\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377"
  "\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\200\200\200(\200\200\200\326\177\177\177\377\177\177\177\331\200\200\200"
  "\314\200\200\200\314\200\200\200\314\200\200\200\322\200\200\200\374\200"
  "\200\200\352~~~G\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000"
  "\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000zzz\027\200\200\200:\200\200\200D\200\200\200Huuu\030\177\177"
  "\177\317\177\177\177\345}}}=\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200"
  "\200\200$\200\200\200\314\200\200\200\356\200\200\200\032\000\000\000\000\000\000\000\000"
  "\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\204\200\200\200\342\200\200"
  "\200\352\177\177\177\307\177\177\177\273\200\200\200\266}}}p\177\177\177"
  "\377~~~I\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200"
  "\200\040\200\200\200\366\177\177\177\217\000\000\000\000\000\000\000\000\177\177\177\377\000"
  "\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200"
  "\002\200\200\200<~~~U~~~\177zzz,\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177"
  "\217\200\200\200\360\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\200\200\200\276\177\177\177\303\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000|||)"
  "\200\200\200\332\177\177\177\377\177\177\177\377\177\177\177\377\177\177"
  "\177\327|||)\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\231\177\177\177\345\000\000"
  "\000\000\000\000\000\000\000\000\000\000\200\200\200\034}}}\063{{{\037\000\000\000\000\000\000\000\000\000\000\000\000\200"
  "\200\200\260\200\200\200\316\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\000\000\000\000\200\200\200\022\200\200\200\344~~~\376"
  "\200\200\200\222~~~E\177\177\177\213\177\177\177\375\177\177\177\335ttt\013"
  "\000\000\000\000\000\000\000\000\177\177\177\231\177\177\177\345\000\000\000\000xxx\021\177\177\177"
  "\263~~~\376\177\177\177\377\177\177\177\377\177\177\177\305|||!\000\000\000\000\200"
  "\200\200\260\200\200\200\316\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\000\000\000\000}}};\177\177\177\323\200\200\200|\000\000"
  "\000\000\000\000\000\000\000\000\000\000\177\177\177\201\177\177\177\377\200\200\200Z\000\000\000\000"
  "\000\000\000\000\177\177\177\231\177\177\177\345\200\200\200\004\177\177\177\307\177"
  "\177\177\377\177\177\177\273\200\200\200d\177\177\177\237\177\177\177\377"
  "\177\177\177\335ttt\013\200\200\200\260\200\200\200\316\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\200\200\200"
  "\066\177\177\177\375\200\200\200\016\000\000\000\000\000\000\000\000\000\000\000\000|||'\177\177\177"
  "\377~~~{\000\000\000\000\000\000\000\000\177\177\177\231\177\177\177\345\200\200\200.\200"
  "\200\200\326\200\200\200\252\200\200\200\002\000\000\000\000\000\000\000\000\200\200\200\202"
  "\177\177\177\377\200\200\200f\200\200\200\260\200\200\200\316\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\200"
  "\200\200(\177\177\177\377\200\200\200J\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200"
  "R\177\177\177\377\200\200\200Z\000\000\000\000\000\000\000\000\177\177\177\231\177\177\177"
  "\345\200\200\200\040\200\200\200\374\200\200\200\032\000\000\000\000\000\000\000\000\000\000\000"
  "\000zzz\027\177\177\177\377\177\177\177\217\200\200\200\260\200\200\200\316"
  "\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000\000\000\000\000\200\200\200\320\200\200\200\332\200\200\200D\200\200\200"
  "\022\200\200\200L\177\177\177\347\177\177\177\351ttt\013\000\000\000\000\000\000\000\000\177"
  "\177\177\231\177\177\177\345zzz\031\177\177\177\377~~~S\000\000\000\000\000\000\000\000\000"
  "\000\000\000}}}\061\177\177\177\377~~~u\200\200\200\260\200\200\200\316\000\000\000\000"
  "\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000"
  "\000\000\000\000\200\200\200\040\200\200\200\326\177\177\177\377\177\177\177\377\177"
  "\177\177\377\200\200\200\342\200\200\200\066\000\000\000\000\000\000\000\000\000\000\000\000\177\177"
  "\177\223\177\177\177\353\000\000\000\000\200\200\200\314\200\200\200\326}}}-\000\000"
  "\000\000|||#\200\200\200\316\200\200\200\372{{{\037\177\177\177\267\177\177\177"
  "\307\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200\002~~~C~~~c~~~Uqqq\011\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000~~~k\177\177\177\377\200\200\200L~~~o\200\200\200\342"
  "\177\177\177\377\177\177\177\361~~~\376\200\200\200\366~~~[\200\200\200\016"
  "\200\200\200\354\200\200\200\236\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "fff\005\200\200\200\324\177\177\177\261\200\200\200X\200\200\200(\200\200\200"
  "\020|||)~~~a\177\177\177\367\177\177\177\373\177\177\177\203\200\200\200\010"
  "\200\200\200^\177\177\177\201\200\200\200l{{{\037\200\200\200\012\200\200\200"
  "\252\200\200\200\372}}}/\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000"
  "\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "qqq\011\200\200\200`\177\177\177\257\200\200\200\330\200\200\200\356\177\177"
  "\177\325\200\200\200\260\200\200\200\214\177\177\177\363\200\200\200\360"
  "\200\200\200\246\177\177\177\231\177\177\177\231\177\177\177\231\177\177"
  "\177\237\177\177\177\343\200\200\200\374\200\200\200n\000\000\000\000\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000{{{\037\177\177\177\241\200\200\200\334\200\200"
  "\200\356\200\200\200\356\200\200\200\356\177\177\177\343\200\200\200\262"
  "}}}\067\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

