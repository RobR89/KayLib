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

#ifndef KCOLOR_H
#define	KCOLOR_H

class KColor {
public:
  float r, g, b, a;

  KColor() {
    r = g = b = 0;
    a = 255;
  }

  KColor(const float red, const float green, const float blue) {
    r = red;
    g = green;
    b = blue;
    a = 255;
  }

  KColor(const float red, const float green, const float blue, const float alpha) {
    r = red;
    g = green;
    b = blue;
    a = alpha;
  }

  static KColor aliceBlue() {
    return new KColor(240, 248, 255);
  };

  static KColor antiqueWhite() {
    return new KColor(250, 235, 215);
  };

  static KColor aqua() {
    return new KColor(0, 255, 255);
  };

  static KColor aquamarine() {
    return new KColor(127, 255, 212);
  };

  static KColor azure() {
    return new KColor(240, 255, 255);
  };

  static KColor beige() {
    return new KColor(245, 245, 220);
  };

  static KColor bisque() {
    return new KColor(255, 228, 196);
  };

  static KColor black() {
    return new KColor(0, 0, 0);
  };

  static KColor blanchedAlmond() {
    return new KColor(255, 255, 205);
  };

  static KColor blue() {
    return new KColor(0, 0, 255);
  };

  static KColor blueViolet() {
    return new KColor(138, 43, 226);
  };

  static KColor brown() {
    return new KColor(165, 42, 42);
  };

  static KColor burlyWood() {
    return new KColor(222, 184, 135);
  };

  static KColor cadetBlue() {
    return new KColor(95, 158, 160);
  };

  static KColor chartreuse() {
    return new KColor(127, 255, 0);
  };

  static KColor chocolate() {
    return new KColor(210, 105, 30);
  };

  static KColor coral() {
    return new KColor(255, 127, 80);
  };

  static KColor cornflowerBlue() {
    return new KColor(100, 149, 237);
  };

  static KColor cornsilk() {
    return new KColor(255, 248, 220);
  };

  static KColor crimson() {
    return new KColor(220, 20, 60);
  };

  static KColor cyan() {
    return new KColor(0, 255, 255);
  };

  static KColor darkBlue() {
    return new KColor(0, 0, 139);
  };

  static KColor darkCyan() {
    return new KColor(0, 139, 139);
  };

  static KColor darkGoldenrod() {
    return new KColor(184, 134, 11);
  };

  static KColor darkGray() {
    return new KColor(169, 169, 169);
  };

  static KColor darkGreen() {
    return new KColor(0, 100, 0);
  };

  static KColor darkKhaki() {
    return new KColor(189, 183, 107);
  };

  static KColor darkMagena() {
    return new KColor(139, 0, 139);
  };

  static KColor darkOliveGreen() {
    return new KColor(85, 107, 47);
  };

  static KColor darkOrange() {
    return new KColor(255, 140, 0);
  };

  static KColor darkOrchid() {
    return new KColor(153, 50, 204);
  };

  static KColor darkRed() {
    return new KColor(139, 0, 0);
  };

  static KColor darkSalmon() {
    return new KColor(233, 150, 122);
  };

  static KColor darkSeaGreen() {
    return new KColor(143, 188, 143);
  };

  static KColor darkSlateBlue() {
    return new KColor(72, 61, 139);
  };

  static KColor darkSlateGray() {
    return new KColor(40, 79, 79);
  };

  static KColor darkTurquoise() {
    return new KColor(0, 206, 209);
  };

  static KColor darkViolet() {
    return new KColor(148, 0, 211);
  };

  static KColor deepPink() {
    return new KColor(255, 20, 147);
  };

  static KColor deepSkyBlue() {
    return new KColor(0, 191, 255);
  };

  static KColor dimGray() {
    return new KColor(105, 105, 105);
  };

  static KColor dodgerBlue() {
    return new KColor(30, 144, 255);
  };

  static KColor firebrick() {
    return new KColor(178, 34, 34);
  };

  static KColor floralWhite() {
    return new KColor(255, 250, 240);
  };

  static KColor forestGreen() {
    return new KColor(34, 139, 34);
  };

  static KColor fuschia() {
    return new KColor(255, 0, 255);
  };

  static KColor gainsboro() {
    return new KColor(220, 220, 220);
  };

  static KColor ghostWhite() {
    return new KColor(248, 248, 255);
  };

  static KColor gold() {
    return new KColor(255, 215, 0);
  };

  static KColor goldenrod() {
    return new KColor(218, 165, 32);
  };

  static KColor gray() {
    return new KColor(128, 128, 128);
  };

  static KColor green() {
    return new KColor(0, 128, 0);
  };

  static KColor greenYellow() {
    return new KColor(173, 255, 47);
  };

  static KColor honeydew() {
    return new KColor(240, 255, 240);
  };

  static KColor hotPink() {
    return new KColor(255, 105, 180);
  };

  static KColor indianRed() {
    return new KColor(205, 92, 92);
  };

  static KColor indigo() {
    return new KColor(75, 0, 130);
  };

  static KColor ivory() {
    return new KColor(255, 240, 240);
  };

  static KColor khaki() {
    return new KColor(240, 230, 140);
  };

  static KColor lavender() {
    return new KColor(230, 230, 250);
  };

  static KColor lavenderBlush() {
    return new KColor(255, 240, 245);
  };

  static KColor lawnGreen() {
    return new KColor(124, 252, 0);
  };

  static KColor lemonChiffon() {
    return new KColor(255, 250, 205);
  };

  static KColor lightBlue() {
    return new KColor(173, 216, 230);
  };

  static KColor lightCoral() {
    return new KColor(240, 128, 128);
  };

  static KColor lightCyan() {
    return new KColor(224, 255, 255);
  };

  static KColor lightGoldenrodYellow() {
    return new KColor(250, 250, 210);
  };

  static KColor lightGreen() {
    return new KColor(144, 238, 144);
  };

  static KColor lightGray() {
    return new KColor(211, 211, 211);
  };

  static KColor lightPink() {
    return new KColor(255, 182, 193);
  };

  static KColor lightSalmon() {
    return new KColor(255, 160, 122);
  };

  static KColor lightSeaGreen() {
    return new KColor(32, 178, 170);
  };

  static KColor lightSkyBlue() {
    return new KColor(135, 206, 250);
  };

  static KColor lightSlateGray() {
    return new KColor(119, 136, 153);
  };

  static KColor lightSteelBlue() {
    return new KColor(176, 196, 222);
  };

  static KColor lightYellow() {
    return new KColor(255, 255, 224);
  };

  static KColor lime() {
    return new KColor(0, 255, 0);
  };

  static KColor limeGreen() {
    return new KColor(50, 205, 50);
  };

  static KColor linen() {
    return new KColor(250, 240, 230);
  };

  static KColor magenta() {
    return new KColor(255, 0, 255);
  };

  static KColor maroon() {
    return new KColor(128, 0, 0);
  };

  static KColor mediumAquamarine() {
    return new KColor(102, 205, 170);
  };

  static KColor mediumBlue() {
    return new KColor(0, 0, 205);
  };

  static KColor mediumOrchid() {
    return new KColor(186, 85, 211);
  };

  static KColor mediumPurple() {
    return new KColor(147, 112, 219);
  };

  static KColor mediumSeaGreen() {
    return new KColor(60, 179, 113);
  };

  static KColor mediumSlateBlue() {
    return new KColor(123, 104, 238);
  };

  static KColor mediumSpringGreen() {
    return new KColor(0, 250, 154);
  };

  static KColor mediumTurquoise() {
    return new KColor(72, 209, 204);
  };

  static KColor mediumVioletRed() {
    return new KColor(199, 21, 112);
  };

  static KColor midnightBlue() {
    return new KColor(25, 25, 112);
  };

  static KColor mintCream() {
    return new KColor(245, 255, 250);
  };

  static KColor mistyRose() {
    return new KColor(255, 228, 225);
  };

  static KColor moccasin() {
    return new KColor(255, 228, 181);
  };

  static KColor NavajoWhite() {
    return new KColor(255, 222, 173);
  };

  static KColor Navy() {
    return new KColor(0, 0, 128);
  };

  static KColor oldLace() {
    return new KColor(253, 245, 230);
  };

  static KColor olive() {
    return new KColor(128, 128, 0);
  };

  static KColor oliveDrab() {
    return new KColor(107, 142, 45);
  };

  static KColor orange() {
    return new KColor(255, 165, 0);
  };

  static KColor orangeRed() {
    return new KColor(255, 69, 0);
  };

  static KColor orchid() {
    return new KColor(218, 112, 214);
  };

  static KColor paleGoldenrod() {
    return new KColor(238, 232, 170);
  };

  static KColor paleGreen() {
    return new KColor(152, 251, 152);
  };

  static KColor paleTurquoise() {
    return new KColor(175, 238, 238);
  };

  static KColor paleVioletRed() {
    return new KColor(219, 112, 147);
  };

  static KColor papayaWhip() {
    return new KColor(255, 239, 213);
  };

  static KColor peachPuff() {
    return new KColor(255, 218, 155);
  };

  static KColor peru() {
    return new KColor(205, 133, 63);
  };

  static KColor pink() {
    return new KColor(255, 192, 203);
  };

  static KColor plum() {
    return new KColor(221, 160, 221);
  };

  static KColor powderBlue() {
    return new KColor(176, 224, 230);
  };

  static KColor purple() {
    return new KColor(128, 0, 128);
  };

  static KColor red() {
    return new KColor(255, 0, 0);
  };

  static KColor rosyBrown() {
    return new KColor(188, 143, 143);
  };

  static KColor royalBlue() {
    return new KColor(65, 105, 225);
  };

  static KColor saddleBrown() {
    return new KColor(139, 69, 19);
  };

  static KColor salmon() {
    return new KColor(250, 128, 114);
  };

  static KColor sandyBrown() {
    return new KColor(244, 164, 96);
  };

  static KColor seaGreen() {
    return new KColor(46, 139, 87);
  };

  static KColor seashell() {
    return new KColor(255, 245, 238);
  };

  static KColor sienna() {
    return new KColor(160, 82, 45);
  };

  static KColor silver() {
    return new KColor(192, 192, 192);
  };

  static KColor skyBlue() {
    return new KColor(135, 206, 235);
  };

  static KColor slateBlue() {
    return new KColor(106, 90, 205);
  };

  static KColor slateGray() {
    return new KColor(112, 128, 144);
  };

  static KColor snow() {
    return new KColor(255, 250, 250);
  };

  static KColor springGreen() {
    return new KColor(0, 255, 127);
  };

  static KColor steelBlue() {
    return new KColor(70, 130, 180);
  };

  static KColor tan() {
    return new KColor(210, 180, 140);
  };

  static KColor teal() {
    return new KColor(0, 128, 128);
  };

  static KColor thistle() {
    return new KColor(216, 191, 216);
  };

  static KColor tomato() {
    return new KColor(253, 99, 71);
  };

  static KColor turquoise() {
    return new KColor(64, 224, 208);
  };

  static KColor violet() {
    return new KColor(238, 130, 238);
  };

  static KColor wheat() {
    return new KColor(245, 222, 179);
  };

  static KColor white() {
    return new KColor(255, 255, 255);
  };

  static KColor whiteSmoke() {
    return new KColor(245, 245, 245);
  };

  static KColor yellow() {
    return new KColor(255, 255, 0);
  };

  static KColor yellowGreen() {
    return new KColor(154, 205, 50);
  };

};


#endif	/* COLOR_H */

