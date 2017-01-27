/*
 * Copyright 2017 Robert Reinhart.
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
#define KCOLOR_H

namespace KayLib
{

    class KColor
    {
    public:
        unsigned char r, g, b, a;

        KColor()
        {
            r = g = b = 0;
            a = 255;
        }

        KColor(const unsigned char red, const unsigned char green, const unsigned char blue)
        {
            r = red;
            g = green;
            b = blue;
            a = 255;
        }

        KColor(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha)
        {
            r = red;
            g = green;
            b = blue;
            a = alpha;
        }

        static KColor aliceBlue()
        {
            return KColor(240, 248, 255);
        };

        static KColor antiqueWhite()
        {
            return KColor(250, 235, 215);
        };

        static KColor aqua()
        {
            return KColor(0, 255, 255);
        };

        static KColor aquamarine()
        {
            return KColor(127, 255, 212);
        };

        static KColor azure()
        {
            return KColor(240, 255, 255);
        };

        static KColor beige()
        {
            return KColor(245, 245, 220);
        };

        static KColor bisque()
        {
            return KColor(255, 228, 196);
        };

        static KColor black()
        {
            return KColor(0, 0, 0);
        };

        static KColor blanchedAlmond()
        {
            return KColor(255, 255, 205);
        };

        static KColor blue()
        {
            return KColor(0, 0, 255);
        };

        static KColor blueViolet()
        {
            return KColor(138, 43, 226);
        };

        static KColor brown()
        {
            return KColor(165, 42, 42);
        };

        static KColor burlyWood()
        {
            return KColor(222, 184, 135);
        };

        static KColor cadetBlue()
        {
            return KColor(95, 158, 160);
        };

        static KColor chartreuse()
        {
            return KColor(127, 255, 0);
        };

        static KColor chocolate()
        {
            return KColor(210, 105, 30);
        };

        static KColor coral()
        {
            return KColor(255, 127, 80);
        };

        static KColor cornflowerBlue()
        {
            return KColor(100, 149, 237);
        };

        static KColor cornsilk()
        {
            return KColor(255, 248, 220);
        };

        static KColor crimson()
        {
            return KColor(220, 20, 60);
        };

        static KColor cyan()
        {
            return KColor(0, 255, 255);
        };

        static KColor darkBlue()
        {
            return KColor(0, 0, 139);
        };

        static KColor darkCyan()
        {
            return KColor(0, 139, 139);
        };

        static KColor darkGoldenrod()
        {
            return KColor(184, 134, 11);
        };

        static KColor darkGray()
        {
            return KColor(169, 169, 169);
        };

        static KColor darkGreen()
        {
            return KColor(0, 100, 0);
        };

        static KColor darkKhaki()
        {
            return KColor(189, 183, 107);
        };

        static KColor darkMagena()
        {
            return KColor(139, 0, 139);
        };

        static KColor darkOliveGreen()
        {
            return KColor(85, 107, 47);
        };

        static KColor darkOrange()
        {
            return KColor(255, 140, 0);
        };

        static KColor darkOrchid()
        {
            return KColor(153, 50, 204);
        };

        static KColor darkRed()
        {
            return KColor(139, 0, 0);
        };

        static KColor darkSalmon()
        {
            return KColor(233, 150, 122);
        };

        static KColor darkSeaGreen()
        {
            return KColor(143, 188, 143);
        };

        static KColor darkSlateBlue()
        {
            return KColor(72, 61, 139);
        };

        static KColor darkSlateGray()
        {
            return KColor(40, 79, 79);
        };

        static KColor darkTurquoise()
        {
            return KColor(0, 206, 209);
        };

        static KColor darkViolet()
        {
            return KColor(148, 0, 211);
        };

        static KColor deepPink()
        {
            return KColor(255, 20, 147);
        };

        static KColor deepSkyBlue()
        {
            return KColor(0, 191, 255);
        };

        static KColor dimGray()
        {
            return KColor(105, 105, 105);
        };

        static KColor dodgerBlue()
        {
            return KColor(30, 144, 255);
        };

        static KColor firebrick()
        {
            return KColor(178, 34, 34);
        };

        static KColor floralWhite()
        {
            return KColor(255, 250, 240);
        };

        static KColor forestGreen()
        {
            return KColor(34, 139, 34);
        };

        static KColor fuschia()
        {
            return KColor(255, 0, 255);
        };

        static KColor gainsboro()
        {
            return KColor(220, 220, 220);
        };

        static KColor ghostWhite()
        {
            return KColor(248, 248, 255);
        };

        static KColor gold()
        {
            return KColor(255, 215, 0);
        };

        static KColor goldenrod()
        {
            return KColor(218, 165, 32);
        };

        static KColor gray()
        {
            return KColor(128, 128, 128);
        };

        static KColor green()
        {
            return KColor(0, 128, 0);
        };

        static KColor greenYellow()
        {
            return KColor(173, 255, 47);
        };

        static KColor honeydew()
        {
            return KColor(240, 255, 240);
        };

        static KColor hotPink()
        {
            return KColor(255, 105, 180);
        };

        static KColor indianRed()
        {
            return KColor(205, 92, 92);
        };

        static KColor indigo()
        {
            return KColor(75, 0, 130);
        };

        static KColor ivory()
        {
            return KColor(255, 240, 240);
        };

        static KColor khaki()
        {
            return KColor(240, 230, 140);
        };

        static KColor lavender()
        {
            return KColor(230, 230, 250);
        };

        static KColor lavenderBlush()
        {
            return KColor(255, 240, 245);
        };

        static KColor lawnGreen()
        {
            return KColor(124, 252, 0);
        };

        static KColor lemonChiffon()
        {
            return KColor(255, 250, 205);
        };

        static KColor lightBlue()
        {
            return KColor(173, 216, 230);
        };

        static KColor lightCoral()
        {
            return KColor(240, 128, 128);
        };

        static KColor lightCyan()
        {
            return KColor(224, 255, 255);
        };

        static KColor lightGoldenrodYellow()
        {
            return KColor(250, 250, 210);
        };

        static KColor lightGreen()
        {
            return KColor(144, 238, 144);
        };

        static KColor lightGray()
        {
            return KColor(211, 211, 211);
        };

        static KColor lightPink()
        {
            return KColor(255, 182, 193);
        };

        static KColor lightSalmon()
        {
            return KColor(255, 160, 122);
        };

        static KColor lightSeaGreen()
        {
            return KColor(32, 178, 170);
        };

        static KColor lightSkyBlue()
        {
            return KColor(135, 206, 250);
        };

        static KColor lightSlateGray()
        {
            return KColor(119, 136, 153);
        };

        static KColor lightSteelBlue()
        {
            return KColor(176, 196, 222);
        };

        static KColor lightYellow()
        {
            return KColor(255, 255, 224);
        };

        static KColor lime()
        {
            return KColor(0, 255, 0);
        };

        static KColor limeGreen()
        {
            return KColor(50, 205, 50);
        };

        static KColor linen()
        {
            return KColor(250, 240, 230);
        };

        static KColor magenta()
        {
            return KColor(255, 0, 255);
        };

        static KColor maroon()
        {
            return KColor(128, 0, 0);
        };

        static KColor mediumAquamarine()
        {
            return KColor(102, 205, 170);
        };

        static KColor mediumBlue()
        {
            return KColor(0, 0, 205);
        };

        static KColor mediumOrchid()
        {
            return KColor(186, 85, 211);
        };

        static KColor mediumPurple()
        {
            return KColor(147, 112, 219);
        };

        static KColor mediumSeaGreen()
        {
            return KColor(60, 179, 113);
        };

        static KColor mediumSlateBlue()
        {
            return KColor(123, 104, 238);
        };

        static KColor mediumSpringGreen()
        {
            return KColor(0, 250, 154);
        };

        static KColor mediumTurquoise()
        {
            return KColor(72, 209, 204);
        };

        static KColor mediumVioletRed()
        {
            return KColor(199, 21, 112);
        };

        static KColor midnightBlue()
        {
            return KColor(25, 25, 112);
        };

        static KColor mintCream()
        {
            return KColor(245, 255, 250);
        };

        static KColor mistyRose()
        {
            return KColor(255, 228, 225);
        };

        static KColor moccasin()
        {
            return KColor(255, 228, 181);
        };

        static KColor NavajoWhite()
        {
            return KColor(255, 222, 173);
        };

        static KColor Navy()
        {
            return KColor(0, 0, 128);
        };

        static KColor oldLace()
        {
            return KColor(253, 245, 230);
        };

        static KColor olive()
        {
            return KColor(128, 128, 0);
        };

        static KColor oliveDrab()
        {
            return KColor(107, 142, 45);
        };

        static KColor orange()
        {
            return KColor(255, 165, 0);
        };

        static KColor orangeRed()
        {
            return KColor(255, 69, 0);
        };

        static KColor orchid()
        {
            return KColor(218, 112, 214);
        };

        static KColor paleGoldenrod()
        {
            return KColor(238, 232, 170);
        };

        static KColor paleGreen()
        {
            return KColor(152, 251, 152);
        };

        static KColor paleTurquoise()
        {
            return KColor(175, 238, 238);
        };

        static KColor paleVioletRed()
        {
            return KColor(219, 112, 147);
        };

        static KColor papayaWhip()
        {
            return KColor(255, 239, 213);
        };

        static KColor peachPuff()
        {
            return KColor(255, 218, 155);
        };

        static KColor peru()
        {
            return KColor(205, 133, 63);
        };

        static KColor pink()
        {
            return KColor(255, 192, 203);
        };

        static KColor plum()
        {
            return KColor(221, 160, 221);
        };

        static KColor powderBlue()
        {
            return KColor(176, 224, 230);
        };

        static KColor purple()
        {
            return KColor(128, 0, 128);
        };

        static KColor red()
        {
            return KColor(255, 0, 0);
        };

        static KColor rosyBrown()
        {
            return KColor(188, 143, 143);
        };

        static KColor royalBlue()
        {
            return KColor(65, 105, 225);
        };

        static KColor saddleBrown()
        {
            return KColor(139, 69, 19);
        };

        static KColor salmon()
        {
            return KColor(250, 128, 114);
        };

        static KColor sandyBrown()
        {
            return KColor(244, 164, 96);
        };

        static KColor seaGreen()
        {
            return KColor(46, 139, 87);
        };

        static KColor seashell()
        {
            return KColor(255, 245, 238);
        };

        static KColor sienna()
        {
            return KColor(160, 82, 45);
        };

        static KColor silver()
        {
            return KColor(192, 192, 192);
        };

        static KColor skyBlue()
        {
            return KColor(135, 206, 235);
        };

        static KColor slateBlue()
        {
            return KColor(106, 90, 205);
        };

        static KColor slateGray()
        {
            return KColor(112, 128, 144);
        };

        static KColor snow()
        {
            return KColor(255, 250, 250);
        };

        static KColor springGreen()
        {
            return KColor(0, 255, 127);
        };

        static KColor steelBlue()
        {
            return KColor(70, 130, 180);
        };

        static KColor tan()
        {
            return KColor(210, 180, 140);
        };

        static KColor teal()
        {
            return KColor(0, 128, 128);
        };

        static KColor thistle()
        {
            return KColor(216, 191, 216);
        };

        static KColor tomato()
        {
            return KColor(253, 99, 71);
        };

        static KColor turquoise()
        {
            return KColor(64, 224, 208);
        };

        static KColor violet()
        {
            return KColor(238, 130, 238);
        };

        static KColor wheat()
        {
            return KColor(245, 222, 179);
        };

        static KColor white()
        {
            return KColor(255, 255, 255);
        };

        static KColor whiteSmoke()
        {
            return KColor(245, 245, 245);
        };

        static KColor yellow()
        {
            return KColor(255, 255, 0);
        };

        static KColor yellowGreen()
        {
            return KColor(154, 205, 50);
        };

    };

}

#endif /* COLOR_H */

