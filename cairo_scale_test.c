// gcc cairo_scale_test.c -o cairo_scale_test `pkg-config --libs --cflags gtk+-2.0`

#include <gtk/gtk.h>

#define CAIRO_SCALE_TEST_ANTIALIAS


enum AreaType
{
  AREA_NO_SCALE,
  AREA_SCALE_SIMPLE,
  AREA_SCALE_NEAREST_PATTERN,
  AREA_SCALE_PAINT_WITH_ALPHA,
  AREA_MATRIX_NEAREST_PATTERN,
  AREA_SCALE_CLIP,
  AREA_SCALE_ROTATE,
  AREA_SCALE_CLIP_AND_ROTATE,
  AREA_SCALE_CRACK,
  AREA_SCALE_JOIN_GOOD,
  AREA_SCALE_JOIN_BAD,

  AREAS_NUM
};


static const gint SOURCE_WIDTH = 50;
static const gint SOURCE_HEIGHT = 5;

static const gchar const* AREAS_NAMES[AREAS_NUM] =
{
 "No scale",
 "Scale and default filter",
 "Scale and nearest filter",
 "Scale, nearest filter and paint with alpha",
 "Matrix and nearest filter",
 "Scale, nearest filter and clip",
 "Scale, nearest filter and rotate",
 "Scale, nearest filter, rotate and clip",
 "Scale, nearest filter, clip and crack",
 "Crack and join good",
 "Crack and join bad",
};


gboolean do_expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
  enum AreaType type = GPOINTER_TO_UINT(user_data);

  gint width = widget->allocation.width;
  gint height = widget->allocation.height;

  cairo_surface_t *source_surface = g_object_get_data(G_OBJECT(gtk_widget_get_toplevel(widget)),
    "source-surface");
  g_assert(source_surface);

  cairo_t *cr = gdk_cairo_create(widget->window);

  cairo_set_source_rgb(cr, 0.0, 0.2, 0.2);
  cairo_paint(cr);

  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

  #ifndef CAIRO_SCALE_TEST_ANTIALIAS
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
  #endif

  cairo_matrix_t matrix;
  cairo_matrix_init_translate(&matrix, width / 3,  height / 20);
  cairo_translate(cr, width / 3,  height / 20);

  switch(type)
  {
    case AREA_NO_SCALE:
      cairo_set_source_surface(cr, source_surface, 0, 0);
      cairo_paint(cr);
    break;

    case AREA_SCALE_SIMPLE:
      cairo_scale(cr, 2, 10);
      cairo_set_source_surface(cr, source_surface, 0, 0);
      cairo_paint(cr);
    break;

    case AREA_SCALE_NEAREST_PATTERN:
      cairo_scale(cr, 2, 10);
      cairo_set_source_surface(cr, source_surface, 0, 0);
      cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
      cairo_paint(cr);
    break;

    case AREA_SCALE_PAINT_WITH_ALPHA:
      cairo_scale(cr, 2, 10);
      cairo_set_source_surface(cr, source_surface, 0, 0);
      cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
      cairo_paint_with_alpha(cr, 0.5);
    break;

    case AREA_MATRIX_NEAREST_PATTERN:
      cairo_matrix_scale(&matrix, 2, 10);
      cairo_set_matrix(cr, &matrix);
      cairo_set_source_surface(cr, source_surface, 0, 0);
      cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
      cairo_paint(cr);
    break;

    case AREA_SCALE_CLIP:
      cairo_scale(cr, 2, 10);

      cairo_move_to(cr, 0     * SOURCE_WIDTH, 1 * SOURCE_HEIGHT);
      cairo_line_to(cr, 0.25  * SOURCE_WIDTH, 0 * SOURCE_HEIGHT);
      cairo_line_to(cr, 0.75  * SOURCE_WIDTH, 0 * SOURCE_HEIGHT);
      cairo_line_to(cr, 1     * SOURCE_WIDTH, 1 * SOURCE_HEIGHT);
      cairo_close_path(cr);
      cairo_clip(cr);

      cairo_set_source_surface(cr, source_surface, 0, 0);
      cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
      cairo_paint(cr);
    break;

    case AREA_SCALE_ROTATE:
      cairo_rotate(cr, G_PI / 8);

      cairo_scale(cr, 2, 10);

      cairo_set_source_surface(cr, source_surface, 0, 0);
      cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
      cairo_paint(cr);
    break;

    case AREA_SCALE_CLIP_AND_ROTATE:
      cairo_rotate(cr, G_PI / 8);

      cairo_scale(cr, 2, 10);

      cairo_move_to(cr, 0     * SOURCE_WIDTH, 1 * SOURCE_HEIGHT);
      cairo_line_to(cr, 0.25  * SOURCE_WIDTH, 0 * SOURCE_HEIGHT);
      cairo_line_to(cr, 0.75  * SOURCE_WIDTH, 0 * SOURCE_HEIGHT);
      cairo_line_to(cr, 1     * SOURCE_WIDTH, 1 * SOURCE_HEIGHT);
      cairo_close_path(cr);
      cairo_clip(cr);

      cairo_set_source_surface(cr, source_surface, 0, 0);
      cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
      cairo_paint(cr);
    break;

    case AREA_SCALE_CRACK:
      cairo_save(cr);
        cairo_scale(cr, 2, 10);

        cairo_move_to(cr, 0   * SOURCE_WIDTH, 0   * SOURCE_HEIGHT);
        cairo_line_to(cr, 0.6 * SOURCE_WIDTH, 0   * SOURCE_HEIGHT);
        cairo_line_to(cr, 0.4 * SOURCE_WIDTH, 1   * SOURCE_HEIGHT);
        cairo_line_to(cr, 0   * SOURCE_WIDTH, 1   * SOURCE_HEIGHT);
        cairo_close_path(cr);
        cairo_clip(cr);

        cairo_set_source_surface(cr, source_surface, 0, 0);
        cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
        cairo_paint(cr);
      cairo_restore(cr);

      cairo_save(cr);
        cairo_translate(cr, 1, 0);
        cairo_scale(cr, 2, 10);

        cairo_line_to(cr, 0.6 * SOURCE_WIDTH, 0 * SOURCE_HEIGHT);
        cairo_line_to(cr, 1   * SOURCE_WIDTH, 0 * SOURCE_HEIGHT);
        cairo_line_to(cr, 1   * SOURCE_WIDTH, 1 * SOURCE_HEIGHT);
        cairo_line_to(cr, 0.4 * SOURCE_WIDTH, 1 * SOURCE_HEIGHT);
        cairo_close_path(cr);
        cairo_clip(cr);

        cairo_set_source_surface(cr, source_surface, 0, 0);
        cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
        cairo_paint(cr);
      cairo_restore(cr);
    break;

    case AREA_SCALE_JOIN_GOOD:
    case AREA_SCALE_JOIN_BAD:
      cairo_save(cr);
        cairo_rotate(cr, G_PI / 8);

        if(type == AREA_SCALE_JOIN_BAD) cairo_translate(cr, 1, 0);

        cairo_scale(cr, 2, 10);

        cairo_move_to(cr, 0   * SOURCE_WIDTH, 0   * SOURCE_HEIGHT);
        cairo_line_to(cr, 0.6 * SOURCE_WIDTH, 0   * SOURCE_HEIGHT);
        cairo_line_to(cr, 0.4 * SOURCE_WIDTH, 1   * SOURCE_HEIGHT);
        cairo_line_to(cr, 0   * SOURCE_WIDTH, 1   * SOURCE_HEIGHT);
        cairo_close_path(cr);
        cairo_clip(cr);

        cairo_set_source_surface(cr, source_surface, 0, 0);
        cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
        cairo_paint_with_alpha(cr, 0.5);
      cairo_restore(cr);

      cairo_save(cr);
        cairo_rotate(cr, G_PI / 8);

        cairo_scale(cr, 2, 10);

        cairo_line_to(cr, 0.6 * SOURCE_WIDTH, 0 * SOURCE_HEIGHT);
        cairo_line_to(cr, 1   * SOURCE_WIDTH, 0 * SOURCE_HEIGHT);
        cairo_line_to(cr, 1   * SOURCE_WIDTH, 1 * SOURCE_HEIGHT);
        cairo_line_to(cr, 0.4 * SOURCE_WIDTH, 1 * SOURCE_HEIGHT);
        cairo_close_path(cr);
        cairo_clip(cr);

        cairo_set_source_surface(cr, source_surface, 0, 0);
        cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
        cairo_paint_with_alpha(cr, 0.5);
      cairo_restore(cr);
    break;

    default:
      g_assert_not_reached();
    break;
  }

  cairo_destroy(cr);

  return FALSE;
}


void quit_cb(void)
{
  gtk_main_quit();
}


int main(int argc, char **argv)
{
  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  #ifdef CAIRO_SCALE_TEST_ANTIALIAS
    gtk_window_set_title(GTK_WINDOW(window), "Cairo scale test with antialiasing");
  #else
    gtk_window_set_title(GTK_WINDOW(window), "Cairo scale test without antialiasing");
  #endif

  GtkWidget *main_vbox = gtk_vbox_new(FALSE, 5);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(main_vbox));

  { // Source surface -->
    gint i, j;

    cairo_surface_t *source_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SOURCE_WIDTH, SOURCE_HEIGHT);
    g_assert(cairo_surface_status(source_surface) == CAIRO_STATUS_SUCCESS);

    unsigned char *data = cairo_image_surface_get_data(source_surface);

    for(i = 0; i < SOURCE_HEIGHT; i++)
      for(j = 0; j < SOURCE_WIDTH; j++)
      {
        #if 0 //< Gradient.
          *(data + 4 * (j + i * SOURCE_WIDTH) + 0) = i * G_MAXUINT8 / SOURCE_HEIGHT;
          *(data + 4 * (j + i * SOURCE_WIDTH) + 1) = j * G_MAXUINT8 / SOURCE_WIDTH;
          *(data + 4 * (j + i * SOURCE_WIDTH) + 2) = G_MAXUINT8 - i * G_MAXUINT8 / SOURCE_HEIGHT;
        #endif
        #if 1 //< Chess gradient.
          if(j % 2)
          {
           *(data + 4 * (j + i * SOURCE_WIDTH) + 0) = i * G_MAXUINT8 / SOURCE_HEIGHT;
           *(data + 4 * (j + i * SOURCE_WIDTH) + 2) = G_MAXUINT8 - i * G_MAXUINT8 / SOURCE_HEIGHT;
          }
          else
          {
           *(data + 4 * (j + i * SOURCE_WIDTH) + 0) = G_MAXUINT8 - i * G_MAXUINT8 / SOURCE_HEIGHT;
           *(data + 4 * (j + i * SOURCE_WIDTH) + 2) = i * G_MAXUINT8 / SOURCE_HEIGHT;
          }

          if(i % 2)
             *(data + 4 * (j + i * SOURCE_WIDTH) + 1) = j * G_MAXUINT8 / SOURCE_WIDTH;
          else
             *(data + 4 * (j + i * SOURCE_WIDTH) + 1) = G_MAXUINT8 - j * G_MAXUINT8 / SOURCE_WIDTH;

          *(data + 4 * (j + i * SOURCE_WIDTH) + 3) = 0xD0;
        #endif
        #if 0 //< Solid color.
          *(data + 4 * (j + i * SOURCE_WIDTH) + 0) = G_MAXUINT8;
          *(data + 4 * (j + i * SOURCE_WIDTH) + 1) = G_MAXUINT8;
          *(data + 4 * (j + i * SOURCE_WIDTH) + 2) = G_MAXUINT8;
          *(data + 4 * (j + i * SOURCE_WIDTH) + 3) = 0xFF;
        #endif
      }

    cairo_surface_mark_dirty(source_surface);

    g_object_set_data_full(G_OBJECT(gtk_widget_get_toplevel(main_vbox)),
      "source-surface", source_surface, (GDestroyNotify)cairo_surface_destroy);
  } // Source surface <--

  GtkWidget *hbox;

  { // Drawing areas -->
    GtkWidget *area;
    guint i;

    for(i = 0; i < AREAS_NUM; i++)
    {
      hbox = gtk_hbox_new(TRUE, 5);
      gtk_box_pack_start(GTK_BOX(main_vbox), hbox, TRUE, TRUE, 5);
      gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new(AREAS_NAMES[i]), TRUE, TRUE, 5);
      area = gtk_drawing_area_new();
      g_signal_connect(G_OBJECT(area), "expose_event", G_CALLBACK(do_expose), GUINT_TO_POINTER(i));
      gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(area), TRUE, TRUE, 5);
    }
  } // Drawing areas <--

  { // Close button -->
    hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_end(GTK_BOX(main_vbox), hbox, FALSE, FALSE, 0);

    GtkWidget *close_button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
    gtk_box_pack_end(GTK_BOX(hbox), GTK_WIDGET(close_button), FALSE, FALSE, 5);

    g_signal_connect(G_OBJECT(close_button), "clicked", G_CALLBACK(quit_cb), NULL);
  } // Close button <--

  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(quit_cb), NULL);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 900);
  gtk_widget_show_all(window);

  { // Icon -->
    static const char *ico_xpm[] =
    {
      "48 48 47 1", "   c #55AAFF", ".  c #55D4FF", "+  c #80D4FF", "@  c #AAFFFF", "#  c #80FFFF", "$  c #5580AA",
      "%  c #80AAD4", "&  c #80AAFF", "*  c #80D4D4", "=  c #558080", "-  c #AAD4FF", ";  c #D4FFFF", ">  c #2B2B55",
      ",  c #555580", "'  c #FFFFFF", ")  c #002B55", "!  c #2B55AA", "~  c #AAAAD4", "{  c #AAAAAA", "]  c #80AAAA",
      "^  c #000000", "/  c #00002B", "(  c #5580D4", "_  c #AAD4D4", ":  c #2B5580", "<  c #2B2B80", "[  c #2B2B2B",
      "}  c #2B55D4", "|  c #000055", "1  c #D4D4FF", "2  c #5580FF", "3  c #55AAD4", "4  c #2B80AA", "5  c #2B80FF",
      "6  c #0055D4", "7  c #002B80", "8  c #002BAA", "9  c #2B55FF", "0  c #0080FF", "a  c #2BAAFF", "b  c #2B80D4",
      "c  c #0055AA", "d  c #000080", "e  c #002B2B", "f  c #0055FF", "g  c #002BD4", "h  c #0000AA",
      "       .++@@@@@@@@@@@@@@@@##+++++##$%####++++++.", "   &+++++@@@@@@@@@@@@@@@###++#####*$@##@++++++++",
      "      .++#@@@@@@@@@@@######@@@@@##=-##@@#++#@@##", "+++#@@@+++++++++++++##@####@@@##++$####@@@@@@@@#",
      "@@@@@@;@@@@@@@@@@###@#@#@@@@@@@@$%*@@;;;;;@@@@@@", "@@@@@@@@@@@@@@@@@@@@@@@;;;;;;@#+>,;;;;;;;;;;;@@@",
      "++-@@@###+@@@@@@;;;;';;;;;;;;;@)!$++~~;;;;;;@@@@", "++--@@@@@@@@@;@;;;;;{]@;;;;;;;+^/!( -;;;;;;;@@@@",
      "@@;;;;;;;;;@;@;;__+@@&=;;;;;;:/^! .   ';;;;;@@@@", "@;@@;;;;;;@@;;;@((<)/)[[%%;;@:^^ ++++ &;;;;;;@@@",
      "@@@;;;;;@@;;'';;;@@@@+%!^^-@+}|!+@@@@@(=1;;;;@@@", "@;;;;;;@@;;;;;;'';;@@;;+ 2)++  #;;@@++<^!;;;@@@@",
      ";;;;;;;@@;;;;;'';;;;;;;@+@&> ++ @@++++ }};;;;@@@", "@;;;;;@@;;;;;;;;$>:;;;@@-@@34+ +@++.++  2 ;;;@@@",
      ";;@;@-@@;@;;;;@-^[_;@@@+ +@%+&  +         ;;;;;;", "@;;;;@@@;;@;;@@@@+@@-+++ .@@@+++5522555555;;;;;@",
      "@;;;-@@;@@@;;@@@+++++++   @@;++-&555666}6 @;;;;;", "@@;;+@@@@@@;;+++++&     55+@@@+@# 5}6666}  ;;;;;",
      "@;;;+@@@@@@;@++&   22  5}5 @;@@@@+ 677865  @;;;;", "@@@++@@@@@;;@+.   55}59}6+@@@@+@+ . }786    ;;;;",
      "@@@++@@;;;;;@+  555966685+#@+#++++++ 2}}   5;;;;", "@;;++@;;;;;;;@+5555}68869 #@++@@++++ 2552  5;;;;",
      ";;;.+@@;@@@@;@@ 066868259a+++++++++  222   2;;;;", ";;;++@@@@@@@@@@+  5555.  + +++++++ &   2    -;;;",
      ";;;++@@@@@@@@@@++++    +++ 5  (}b       22  +;;;", ";;;+++@@@@@@@@+ &+++.++&  25c88|/62 +   25   ;;;",
      ";;@+++-@+++@+@+    +.    555)76|cb6c6555552  ;@@", ";;@-++-@+++@@++    +++    8}}/6^}6/^^78}552  ;@@",
      ";;;+++++++++++      +++  !^8!^/^^^^^^^|85222 ;@#", ";;;++++++ ++.++   5   +.b^^^^^^^^^^^^^d66522 ;;@",
      ";;@+& &+  +       5     /^^^^^^^^^^^^/|c9}55+;@@", "@;@+&             5    :^^^^^^^^^^/^/d866555@@# ",
      "--@++            2   +(^|d^^^^^^|c8^|8655555;@a ", "+@;;     2       2 +++e^|87/8|}6666785   55 @ a ",
      ";;@;      2        ++>^^|6}65555b}66    525++5a ", "@;@@+   2           6//|762   &&   5   55 5+a .+",
      "@@;@+    222   555 22669a&+    5       559 +a .+", "@@@++     22   555555 2 +  255965555555665@.  .#",
      "+@+ .+  2 2525a 509555   5555666555556665@+ .+.+", " &  .+    255555 55995f955555}8655555665@#+ .++#",
      "   ++@   25595555555f669999559}55655555+#+++.+##", "-++;@@@+  55ff55f5 555ff555550556f0ff65@@#+###@#",
      "@+;@@@@;+ 5596f555559ff9f55588888g66665&#@@@@@@@", "+;@-@;;;;+a599555555ff66f0688888666686}5##@@@@@@",
      "-+ &+@@@;; 5556g65555f5006668gg66g66g665+.#@@@@@", "&+++&@@@;+ 5555555555595968d88c88886}665a@###+#@",
      "+-++++@@@+  555955555566gg88687ddh866655 +##+++.", "+&++& @@@+   5559f5fffff66688dd|d88669555 +++.+@"
    };

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_xpm_data(ico_xpm);
    gtk_window_set_icon(GTK_WINDOW(window), pixbuf);
    g_object_unref(pixbuf);
  } // Icon <--

  gtk_main();

  return 0;
}
