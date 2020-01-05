/* GIMP RGBA C-Source image dump (type_diag_internal.c) */

static const struct {
  guint  	 width;
  guint  	 height;
  guint  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  guint8 	 pixel_data[32 * 24 * 4 + 1];
} type_diag_internal = {
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
  "@\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000}}}\071\177\177\177\303\177\177\177"
  "\303\177\177\177\303\177\177\177\303\177\177\177\303\177\177\177\303\177"
  "\177\177\303\177\177\177\303\177\177\177\303\177\177\177\303\177\177\177"
  "\303\177\177\177\303\177\177\177\303\177\177\177\303\177\177\177\303\177"
  "\177\177\303\177\177\177\303\177\177\177\303\177\177\177\303\177\177\177"
  "\303\177\177\177\303\177\177\177\303qqq\011\000\000\000\000\000\000\000\000\177\177\177\377"
  "\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000~~~K\177\177\177\377\200"
  "\200\200\312\177\177\177\275\177\177\177\275\177\177\177\275\177\177\177"
  "\275\177\177\177\275\177\177\177\275\177\177\177\275\177\177\177\275\177"
  "\177\177\275\177\177\177\275\177\177\177\275\177\177\177\275\177\177\177"
  "\275\177\177\177\275\177\177\177\275\177\177\177\275\177\177\177\275\177"
  "\177\177\275\177\177\177\333\177\177\177\377\200\200\200\014\000\000\000\000\000\000\000"
  "\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000~~~K\177"
  "\177\177\377}}}\065\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\200\200\200t\177\177\177\377\200\200\200\014\000\000\000\000\000"
  "\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000~~"
  "~K\177\177\177\377}}}\065\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\200\200\200D\200\200\200\370\200\200\200\370\200\200\200\370\200\200\200"
  "\370\200\200\200\370\200\200\200\370\200\200\200\370\200\200\200\370\200"
  "\200\200\304\000\000\000\000\000\000\000\000\200\200\200t\177\177\177\377\200\200\200\014"
  "\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000~~~K\177\177\177\377}}}\065\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000~~~G\177\177\177\377\200\200\200\242\200\200\200\210\200\200\200"
  "\210\200\200\200\210\200\200\200\210\200\200\200\210\200\200\200\334\177"
  "\177\177\313\000\000\000\000\000\000\000\000\200\200\200t\177\177\177\377\200\200\200\014"
  "\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000~~~K\177\177\177\377}}}\065\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\200\200\200Z\210\210\210\333\207\207\207\377\207\207\207\326\200\200\200"
  "\070\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\265\177\177\177\313\000\000\000"
  "\000\000\000\000\000\200\200\200t\177\177\177\377\200\200\200\014\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000~~~K\177\177"
  "\177\377}}}\065\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200z\254\254"
  "\254\376\336\336\336\377\226\226\226\366\200\200\200L\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\177\177\177\265\177\177\177\313\000\000\000\000\000\000\000\000\200\200\200"
  "t\177\177\177\377\200\200\200\014\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000\177\177\177\377\000\000\000\000\200\200\200,\177\177\177\233\177\177\177"
  "\377}}}\217\200\200\200*\000\000\000\000\200\200\200\036\177\177\177\323\200\200\200"
  "\356\200\200\200\202\273\273\273\375\377\377\377\377\240\240\240\363\200"
  "\200\200L\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\265\177\177\177\313"
  "\000\000\000\000\000\000\000\000\200\200\200t\177\177\177\377\200\200\200\014\000\000\000\000\000\000"
  "\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000~~~e\222\222"
  "\222\372\264\264\264\377\220\220\220\371}}}h}}}?\200\200\200\336~~~oxxx\021"
  "~~~{\212\212\212\371\226\226\226\377\205\205\205\371\200\200\200L\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\177\177\177\265\177\177\177\313\000\000\000\000\000\000\000\000"
  "\200\200\200t\177\177\177\377\200\200\200\014\000\000\000\000\000\000\000\000\177\177\177"
  "\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000~~~e\255\255\255\370\377\377"
  "\377\377\253\253\253\366}}}|\200\200\200\314\200\200\200f\000\000\000\000\000\000\000\000"
  "\200\200\200\036}}}t\177\177\177\377}}}j\200\200\200\022\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\177\177\177\265\177\177\177\313\000\000\000\000\000\000\000\000\200\200\200"
  "t\177\177\177\377\200\200\200\014\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000"
  "\000\000\000\000\177\177\177\377\000\000\000\000~~~e\230\230\230\373\305\305\305\377\227"
  "\227\227\372~~~a\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000~~~G\177\177\177"
  "\377\200\200\200\374\177\177\177\373\177\177\177\373\177\177\177\373\177"
  "\177\177\373\177\177\177\373~~~\376\177\177\177\313\000\000\000\000\000\000\000\000\200\200"
  "\200t\177\177\177\377\200\200\200\014\000\000\000\000\000\000\000\000\177\177\177\377\000\000"
  "\000\000\000\000\000\000\177\177\177\377\000\000\000\000}}}\067\203\203\203\261\202\202\202\377"
  "\203\203\203\250}}}\065\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\200\200\200"
  "$\177\177\177\205\177\177\177\205\177\177\177\205\177\177\177\205\177\177"
  "\177\205\177\177\177\205\177\177\177\205\177\177\177\205~~~i\000\000\000\000\000\000"
  "\000\000\200\200\200t\177\177\177\377\200\200\200\014\000\000\000\000\000\000\000\000\177\177"
  "\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000~~~K\177\177\177"
  "\377}}}\065\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\200\200\200t\177\177\177\377\200\200\200\014\000\000\000\000\000\000\000\000\177"
  "\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000~~~K\177\177"
  "\177\377\177\177\177\357\177\177\177\353\177\177\177\353\177\177\177\353"
  "\177\177\177\353\177\177\177\353\177\177\177\353\177\177\177\353\177\177"
  "\177\353\177\177\177\353\177\177\177\353\177\177\177\353\177\177\177\353"
  "\177\177\177\353\177\177\177\353\177\177\177\353\177\177\177\353\177\177"
  "\177\353\177\177\177\353\200\200\200\364\177\177\177\377\200\200\200\014\000"
  "\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000"
  "\000\000\000}}}+\177\177\177\225\177\177\177\225\177\177\177\225\177\177\177\225"
  "\177\177\177\225\177\177\177\225\177\177\177\225\177\177\177\225\177\177"
  "\177\225\177\177\177\225\177\177\177\225\177\177\177\225\177\177\177\225"
  "\177\177\177\225\177\177\177\225\177\177\177\225\177\177\177\225\177\177"
  "\177\225\177\177\177\225\177\177\177\225\177\177\177\225\177\177\177\225"
  "\200\200\200\006\000\000\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177"
  "\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\177\177\177\377\000\000\000\000\000\000\000\000\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177"
  "\177\177\377\177\177\177\377\177\177\177\377\177\177\177\377\177\177\177"
  "\377\177\177\177\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};
