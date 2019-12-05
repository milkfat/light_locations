#ifndef LIGHTS_CONFIG_H
#define LIGHTS_CONFIG_H

CRGB leds[NUM_LEDS+1]; //create led object in memory

CRGB canvas[NUM_LEDS]; //object for teh drawing

char current_light_sketch = '-';
boolean light_pulse_enabled = 0;
byte global_brightness = 32;
boolean filter_particles = 1;

String display_text = "";
boolean text_mask = 0;
boolean invert_mask = 0;
boolean text_filter = 1;
boolean text_rotate = 0;
#define NUM_TEXT_EFFECTS 5
int text_effect = 0;
byte led_mask[NUM_LEDS];
byte led_mask2[NUM_LEDS];
int text_color = -1;
byte text_saturation = 255;
byte text_brightness = 96;
boolean scrolling_text_enabled = 0;
boolean drawing_enabled = 0;
byte current_font = 0;
#define NUM_FONTS 8;
unsigned long text_shake_time = 0;
int ystp = 0;
byte font_scaler = 48;
byte debug_scaler = 128;
byte camera_scaler = 200;
byte screen_scaler = 100;
byte default_color = 0;
byte default_saturation = 255;


//CONFIG STUFF
class CONFIG_OPTION {
  public:
    String name;
    String key;
    String val;
    byte* by;
    boolean* bo;
    char* c;
    int* i;
    String* s;
    byte type;
    byte option_type;
    int range_min;
    int range_max;
    String value();
    CONFIG_OPTION(String,String,byte&,byte,int,int);
    CONFIG_OPTION(String,String,boolean&,byte,int,int);
    CONFIG_OPTION(String,String,char&,byte,int,int);
    CONFIG_OPTION(String,String,int&,byte,int,int);
    CONFIG_OPTION(String,String,String&,byte,int,int);
    CONFIG_OPTION(String,String,const char*,byte,int,int);
};

//option types
#define UNDEFINED -1
#define CHECKBOX 0
#define RANGE 1
#define TEXT 2
#define BUTTON 3


//datatypes for options
#define BYTE 0
#define BOOL 1
#define CHAR 2
#define INT 3
#define STRING 4

CONFIG_OPTION::CONFIG_OPTION(String xname, String xkey, byte& x, byte xoption_type = UNDEFINED, int xrangemin = 0, int xrangemax = 255) {
  name = xname;
  key = xkey;
  by = &x;
  type = BYTE;
  option_type = xoption_type;
  range_min = xrangemin;
  range_max = xrangemax;
}
CONFIG_OPTION::CONFIG_OPTION(String xname, String xkey, boolean& x, byte xoption_type = UNDEFINED, int xrangemin = 0, int xrangemax = 255) {
  name = xname;
  key = xkey;
  bo = &x;
  type = BOOL;
  option_type = xoption_type;
  range_min = xrangemin;
  range_max = xrangemax;
}
CONFIG_OPTION::CONFIG_OPTION(String xname, String xkey, char& x, byte xoption_type = UNDEFINED, int xrangemin = 0, int xrangemax = 255) {
  name = xname;
  key = xkey;
  c = &x;
  type = CHAR;
  option_type = xoption_type;
  range_min = xrangemin;
  range_max = xrangemax;
}
CONFIG_OPTION::CONFIG_OPTION(String xname, String xkey, int& x, byte xoption_type = UNDEFINED, int xrangemin = 0, int xrangemax = 255) {
  name = xname;
  key = xkey;
  i = &x;
  type = INT;
  option_type = xoption_type;
  range_min = xrangemin;
  range_max = xrangemax;
}
CONFIG_OPTION::CONFIG_OPTION(String xname, String xkey, String& x, byte xoption_type = UNDEFINED, int xrangemin = 0, int xrangemax = 255) {
  name = xname;
  key = xkey;
  s = &x;
  type = STRING;
  option_type = xoption_type;
  range_min = xrangemin;
  range_max = xrangemax;
}
CONFIG_OPTION::CONFIG_OPTION(String xname, String xkey, const char x[], byte xoption_type = UNDEFINED, int xrangemin = 0, int xrangemax = 255) {
  name = xname;
  key = xkey;
  val = x;
  type = STRING;
  option_type = xoption_type;
  range_min = xrangemin;
  range_max = xrangemax;
}


String CONFIG_OPTION::value() {
  String rval = "";
  switch (type)
  {
    case BYTE:
    {
      rval = (String)*by;
      break;
    }
    case BOOL:
    {
      rval = (String)*bo;
      break;
    }
    case CHAR:
    {
      rval = (String)*c;
      break;
    }
    case INT:
    {
      rval = (String)*i;
      break;
    }
    case STRING:
    {
      if(val.length() > 0) {
        rval = val;
      } else {
        rval = (String)*s;
      }
      break;
    }
  }
  return rval;
}


CONFIG_OPTION config_options[]
{
  //.name, .key, .by, .type, .range_min, .range_max
  CONFIG_OPTION("Current Sketch","sk",current_light_sketch),
  CONFIG_OPTION("Global Brightness","gb",global_brightness,RANGE),
  CONFIG_OPTION("Filter Particles","fp",filter_particles,CHECKBOX),
  CONFIG_OPTION("Light Pulses to Beat","lp",light_pulse_enabled,CHECKBOX),
  CONFIG_OPTION("Text Mask","tm",text_mask,CHECKBOX),
  CONFIG_OPTION("Invert Mask","im",text_mask,CHECKBOX),
  CONFIG_OPTION("Text Filter","tf",text_filter,CHECKBOX),
  CONFIG_OPTION("Text Rotate","tr",text_rotate,CHECKBOX),
  CONFIG_OPTION("Text Color","tc",text_color,RANGE,-1),
  CONFIG_OPTION("Text Saturation","tsa",text_saturation,RANGE),
  CONFIG_OPTION("Text Brightness","tb",text_brightness,RANGE),
  CONFIG_OPTION("Font Scale","fs",font_scaler,RANGE),
  CONFIG_OPTION("Debug Scale","ds",debug_scaler,RANGE),
  CONFIG_OPTION("Camera Z Position","cs",camera_scaler,RANGE),
  CONFIG_OPTION("Screen Z Position","ss",screen_scaler,RANGE),
  CONFIG_OPTION("Default Color","dc",default_color,RANGE),
  CONFIG_OPTION("Default Saturation","dsa",default_saturation,RANGE),
  CONFIG_OPTION("Current Font","cff",current_font),
  CONFIG_OPTION("Scrolling Text","st",scrolling_text_enabled,CHECKBOX),
  CONFIG_OPTION("Draw","dr",drawing_enabled,CHECKBOX),
  CONFIG_OPTION("Display Text","text",display_text,TEXT),
  CONFIG_OPTION("EFFECT VARIATION","fl","v",BUTTON),
  CONFIG_OPTION("RESET ARDUINO","fl","R",BUTTON),
  CONFIG_OPTION("FORCE RESET ARDUINO","fl","X",BUTTON),
  CONFIG_OPTION("RESET SKETCH","fl","r",BUTTON),
  CONFIG_OPTION("RESET WIFI","rw","rw",BUTTON),
  CONFIG_OPTION("FLASH!","fl","F",BUTTON),
  CONFIG_OPTION("Leafs","sk","a",BUTTON),
  CONFIG_OPTION("Grow Circle","sk","b",BUTTON),
  CONFIG_OPTION("Balls","sk","c",BUTTON),
  CONFIG_OPTION("Frequency Visualizer","sk","d",BUTTON),
  CONFIG_OPTION("Sound Level Meter","sk","e",BUTTON),
  CONFIG_OPTION("2D Test","sk","f",BUTTON),
  CONFIG_OPTION("Lightning","sk","g",BUTTON),
  CONFIG_OPTION("2D Balls","sk","h",BUTTON),
  CONFIG_OPTION("Shapes","sk","i",BUTTON),
  CONFIG_OPTION("Lightning2","sk","j",BUTTON),
  CONFIG_OPTION("Neon","sk","k",BUTTON),
  CONFIG_OPTION("3D Test","sk","l",BUTTON),
  CONFIG_OPTION("Light Bar","sk","m",BUTTON),
  CONFIG_OPTION("Droplet","sk","n",BUTTON),
  CONFIG_OPTION("Wave","sk","o",BUTTON),
  CONFIG_OPTION("Fire","sk","p",BUTTON),
  CONFIG_OPTION("Metaballs","sk","q",BUTTON),
  CONFIG_OPTION("OFF","sk","-",BUTTON),
  CONFIG_OPTION("TEXT SHAKE","ts","1",BUTTON),
  CONFIG_OPTION("CHANGE FONT","cf","1",BUTTON),
  CONFIG_OPTION("CHANGE TEXT EFFECT","te","1",BUTTON),
  CONFIG_OPTION("REQUEST CONFIG","conf","please",BUTTON)
};

#endif