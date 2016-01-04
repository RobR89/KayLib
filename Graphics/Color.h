/*
 * Copyright 2016 Robert Reinhart.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COLOR_H
#define	COLOR_H

class Color {
public:
  float r, g, b, a;

  Color() {
    r = g = b = 0;
    a = 255;
  }

  Color(const float red, const float green, const float blue) {
    r = red;
    g = green;
    b = blue;
    a = 255;
  }

  Color(const float red, const float green, const float blue, const float alpha) {
    r = red;
    g = green;
    b = blue;
    a = alpha;
  }

  static Color AliceBlue() {
    return new Color(240, 248, 255);
  };

  static Color AntiqueWhite() {
    return new Color(250, 235, 215);
  };

  static Color Aqua() {
    return new Color(0, 255, 255);
  };

  static Color Aquamarine() {
    return new Color(127, 255, 212);
  };

  static Color Azure() {
    return new Color(240, 255, 255);
  };

  static Color Beige() {
    return new Color(245, 245, 220);
  };

  static Color Bisque() {
    return new Color(255, 228, 196);
  };

  static Color Black() {
    return new Color(0, 0, 0);
  };

  static Color BlanchedAlmond() {
    return new Color(255, 255, 205);
  };

  static Color Blue() {
    return new Color(0, 0, 255);
  };

  static Color BlueViolet() {
    return new Color(138, 43, 226);
  };

  static Color Brown() {
    return new Color(165, 42, 42);
  };

  static Color BurlyWood() {
    return new Color(222, 184, 135);
  };

  static Color CadetBlue() {
    return new Color(95, 158, 160);
  };

  static Color Chartreuse() {
    return new Color(127, 255, 0);
  };

  static Color Chocolate() {
    return new Color(210, 105, 30);
  };

  static Color Coral() {
    return new Color(255, 127, 80);
  };

  static Color CornflowerBlue() {
    return new Color(100, 149, 237);
  };

  static Color Cornsilk() {
    return new Color(255, 248, 220);
  };

  static Color Crimson() {
    return new Color(220, 20, 60);
  };

  static Color Cyan() {
    return new Color(0, 255, 255);
  };

  static Color DarkBlue() {
    return new Color(0, 0, 139);
  };

  static Color DarkCyan() {
    return new Color(0, 139, 139);
  };

  static Color DarkGoldenrod() {
    return new Color(184, 134, 11);
  };

  static Color DarkGray() {
    return new Color(169, 169, 169);
  };

  static Color DarkGreen() {
    return new Color(0, 100, 0);
  };

  static Color DarkKhaki() {
    return new Color(189, 183, 107);
  };

  static Color DarkMagena() {
    return new Color(139, 0, 139);
  };

  static Color DarkOliveGreen() {
    return new Color(85, 107, 47);
  };

  static Color DarkOrange() {
    return new Color(255, 140, 0);
  };

  static Color DarkOrchid() {
    return new Color(153, 50, 204);
  };

  static Color DarkRed() {
    return new Color(139, 0, 0);
  };

  static Color DarkSalmon() {
    return new Color(233, 150, 122);
  };

  static Color DarkSeaGreen() {
    return new Color(143, 188, 143);
  };

  static Color DarkSlateBlue() {
    return new Color(72, 61, 139);
  };

  static Color DarkSlateGray() {
    return new Color(40, 79, 79);
  };

  static Color DarkTurquoise() {
    return new Color(0, 206, 209);
  };

  static Color DarkViolet() {
    return new Color(148, 0, 211);
  };

  static Color DeepPink() {
    return new Color(255, 20, 147);
  };

  static Color DeepSkyBlue() {
    return new Color(0, 191, 255);
  };

  static Color DimGray() {
    return new Color(105, 105, 105);
  };

  static Color DodgerBlue() {
    return new Color(30, 144, 255);
  };

  static Color Firebrick() {
    return new Color(178, 34, 34);
  };

  static Color FloralWhite() {
    return new Color(255, 250, 240);
  };

  static Color ForestGreen() {
    return new Color(34, 139, 34);
  };

  static Color Fuschia() {
    return new Color(255, 0, 255);
  };

  static Color Gainsboro() {
    return new Color(220, 220, 220);
  };

  static Color GhostWhite() {
    return new Color(248, 248, 255);
  };

  static Color Gold() {
    return new Color(255, 215, 0);
  };

  static Color Goldenrod() {
    return new Color(218, 165, 32);
  };

  static Color Gray() {
    return new Color(128, 128, 128);
  };

  static Color Green() {
    return new Color(0, 128, 0);
  };

  static Color GreenYellow() {
    return new Color(173, 255, 47);
  };

  static Color Honeydew() {
    return new Color(240, 255, 240);
  };

  static Color HotPink() {
    return new Color(255, 105, 180);
  };

  static Color IndianRed() {
    return new Color(205, 92, 92);
  };

  static Color Indigo() {
    return new Color(75, 0, 130);
  };

  static Color Ivory() {
    return new Color(255, 240, 240);
  };

  static Color Khaki() {
    return new Color(240, 230, 140);
  };

  static Color Lavender() {
    return new Color(230, 230, 250);
  };

  static Color LavenderBlush() {
    return new Color(255, 240, 245);
  };

  static Color LawnGreen() {
    return new Color(124, 252, 0);
  };

  static Color LemonChiffon() {
    return new Color(255, 250, 205);
  };

  static Color LightBlue() {
    return new Color(173, 216, 230);
  };

  static Color LightCoral() {
    return new Color(240, 128, 128);
  };

  static Color LightCyan() {
    return new Color(224, 255, 255);
  };

  static Color LightGoldenrodYellow() {
    return new Color(250, 250, 210);
  };

  static Color LightGreen() {
    return new Color(144, 238, 144);
  };

  static Color LightGray() {
    return new Color(211, 211, 211);
  };

  static Color LightPink() {
    return new Color(255, 182, 193);
  };

  static Color LightSalmon() {
    return new Color(255, 160, 122);
  };

  static Color LightSeaGreen() {
    return new Color(32, 178, 170);
  };

  static Color LightSkyBlue() {
    return new Color(135, 206, 250);
  };

  static Color LightSlateGray() {
    return new Color(119, 136, 153);
  };

  static Color LightSteelBlue() {
    return new Color(176, 196, 222);
  };

  static Color LightYellow() {
    return new Color(255, 255, 224);
  };

  static Color Lime() {
    return new Color(0, 255, 0);
  };

  static Color LimeGreen() {
    return new Color(50, 205, 50);
  };

  static Color Linen() {
    return new Color(250, 240, 230);
  };

  static Color Magenta() {
    return new Color(255, 0, 255);
  };

  static Color Maroon() {
    return new Color(128, 0, 0);
  };

  static Color MediumAquamarine() {
    return new Color(102, 205, 170);
  };

  static Color MediumBlue() {
    return new Color(0, 0, 205);
  };

  static Color MediumOrchid() {
    return new Color(186, 85, 211);
  };

  static Color MediumPurple() {
    return new Color(147, 112, 219);
  };

  static Color MediumSeaGreen() {
    return new Color(60, 179, 113);
  };

  static Color MediumSlateBlue() {
    return new Color(123, 104, 238);
  };

  static Color MediumSpringGreen() {
    return new Color(0, 250, 154);
  };

  static Color MediumTurquoise() {
    return new Color(72, 209, 204);
  };

  static Color MediumVioletRed() {
    return new Color(199, 21, 112);
  };

  static Color MidnightBlue() {
    return new Color(25, 25, 112);
  };

  static Color MintCream() {
    return new Color(245, 255, 250);
  };

  static Color MistyRose() {
    return new Color(255, 228, 225);
  };

  static Color Moccasin() {
    return new Color(255, 228, 181);
  };

  static Color NavajoWhite() {
    return new Color(255, 222, 173);
  };

  static Color Navy() {
    return new Color(0, 0, 128);
  };

  static Color OldLace() {
    return new Color(253, 245, 230);
  };

  static Color Olive() {
    return new Color(128, 128, 0);
  };

  static Color OliveDrab() {
    return new Color(107, 142, 45);
  };

  static Color Orange() {
    return new Color(255, 165, 0);
  };

  static Color OrangeRed() {
    return new Color(255, 69, 0);
  };

  static Color Orchid() {
    return new Color(218, 112, 214);
  };

  static Color PaleGoldenrod() {
    return new Color(238, 232, 170);
  };

  static Color PaleGreen() {
    return new Color(152, 251, 152);
  };

  static Color PaleTurquoise() {
    return new Color(175, 238, 238);
  };

  static Color PaleVioletRed() {
    return new Color(219, 112, 147);
  };

  static Color PapayaWhip() {
    return new Color(255, 239, 213);
  };

  static Color PeachPuff() {
    return new Color(255, 218, 155);
  };

  static Color Peru() {
    return new Color(205, 133, 63);
  };

  static Color Pink() {
    return new Color(255, 192, 203);
  };

  static Color Plum() {
    return new Color(221, 160, 221);
  };

  static Color PowderBlue() {
    return new Color(176, 224, 230);
  };

  static Color Purple() {
    return new Color(128, 0, 128);
  };

  static Color Red() {
    return new Color(255, 0, 0);
  };

  static Color RosyBrown() {
    return new Color(188, 143, 143);
  };

  static Color RoyalBlue() {
    return new Color(65, 105, 225);
  };

  static Color SaddleBrown() {
    return new Color(139, 69, 19);
  };

  static Color Salmon() {
    return new Color(250, 128, 114);
  };

  static Color SandyBrown() {
    return new Color(244, 164, 96);
  };

  static Color SeaGreen() {
    return new Color(46, 139, 87);
  };

  static Color Seashell() {
    return new Color(255, 245, 238);
  };

  static Color Sienna() {
    return new Color(160, 82, 45);
  };

  static Color Silver() {
    return new Color(192, 192, 192);
  };

  static Color SkyBlue() {
    return new Color(135, 206, 235);
  };

  static Color SlateBlue() {
    return new Color(106, 90, 205);
  };

  static Color SlateGray() {
    return new Color(112, 128, 144);
  };

  static Color Snow() {
    return new Color(255, 250, 250);
  };

  static Color SpringGreen() {
    return new Color(0, 255, 127);
  };

  static Color SteelBlue() {
    return new Color(70, 130, 180);
  };

  static Color Tan() {
    return new Color(210, 180, 140);
  };

  static Color Teal() {
    return new Color(0, 128, 128);
  };

  static Color Thistle() {
    return new Color(216, 191, 216);
  };

  static Color Tomato() {
    return new Color(253, 99, 71);
  };

  static Color Turquoise() {
    return new Color(64, 224, 208);
  };

  static Color Violet() {
    return new Color(238, 130, 238);
  };

  static Color Wheat() {
    return new Color(245, 222, 179);
  };

  static Color White() {
    return new Color(255, 255, 255);
  };

  static Color WhiteSmoke() {
    return new Color(245, 245, 245);
  };

  static Color Yellow() {
    return new Color(255, 255, 0);
  };

  static Color YellowGreen() {
    return new Color(154, 205, 50);
  };

};


#endif	/* COLOR_H */

