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
/* 
 * @referance based on http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
 */

#ifndef PERLINNOISE_H
#define	PERLINNOISE_H

namespace KayLib {

class PerlinNoise {
private:
  int numOctaves;
  double persistence;
  double frequency;
  // can change the prime numbers to other prime numbers to
  // create different noise functions/patterns
  double prime[4];

public:

  PerlinNoise() {
    numOctaves = 4;
    persistence = 0.25f;
    frequency = 1;
    prime[0] = 15731;
    prime[1] = 789221;
    prime[2] = 1376312589;
    prime[3] = 1073741824;
  }

  PerlinNoise(int Octaves, double nPersistence) {
    numOctaves = Octaves;
    persistence = nPersistence;
    frequency = 1;
    prime[0] = 15731;
    prime[1] = 789221;
    prime[2] = 1376312589;
    prime[3] = 1073741824;
  }

  PerlinNoise(int Octaves, double nPersistence, double prime1, double prime2, double prime3, double prime4) {
    numOctaves = Octaves;
    persistence = nPersistence;
    frequency = 1;
    prime[0] = prime1;
    prime[1] = prime2;
    prime[2] = prime3;
    prime[3] = prime4;
  }

  double perlinNoise(double x) {
    double total = 0;
    double amplitude = 1;
    for (int i = 0; i < numOctaves; i++) {
      total += interpolatedNoise(x * frequency) * amplitude;
      x *= 2;
      amplitude *= persistence;
    }
    return total;
  }

  double perlinNoise(double x, double y) {
    double total = 0;
    double amplitude = 1;
    for (int i = 0; i < numOctaves; i++) {
      total += interpolatedNoise(x * frequency, y * frequency) * amplitude;
      x *= 2;
      y *= 2;
      amplitude *= persistence;
    }

    return total;
  }

  double perlinNoise(double x, double y, double z) {
    double total = 0;
    double amplitude = 1;
    for (int i = 0; i < numOctaves; i++) {
      total += interpolatedNoise(x * frequency, y * frequency, z * frequency) * amplitude;
      x *= 2;
      y *= 2;
      z *= 2;
      amplitude *= persistence;
    }

    return total;
  }

private:

  double interpolatedNoise(double x) {
    int integer_X = (int) x;
    double fractional_X = (double) x - integer_X;

    double v1 = smoothNoise(integer_X);
    double v2 = smoothNoise(integer_X + 1);

    return Interpolate(v1, v2, fractional_X);
  }

  double interpolatedNoise(double x, double y) {
    int integer_X = (int) x;
    double fractional_X = (double) x - integer_X;
    int integer_Y = (int) y;
    double fractional_Y = (double) y - integer_Y;

    double v1 = smoothNoise(integer_X, integer_Y);
    double v2 = smoothNoise(integer_X + 1, integer_Y);
    double v3 = smoothNoise(integer_X, integer_Y + 1);
    double v4 = smoothNoise(integer_X + 1, integer_Y + 1);

    double i1 = Interpolate(v1, v2, fractional_X);
    double i2 = Interpolate(v3, v4, fractional_X);

    return Interpolate(i1, i2, fractional_Y);
  }

  double interpolatedNoise(double x, double y, double z) {
    int integer_X = (int) x;
    double fractional_X = (double) x - integer_X;
    int integer_Y = (int) y;
    double fractional_Y = (double) y - integer_Y;
    int integer_Z = (int) z;
    double fractional_Z = (double) z - integer_Z;

    double v1 = smoothNoise(integer_X, integer_Y, integer_Z);
    double v2 = smoothNoise(integer_X + 1, integer_Y, integer_Z);
    double v3 = smoothNoise(integer_X, integer_Y + 1, integer_Z);
    double v4 = smoothNoise(integer_X + 1, integer_Y + 1, integer_Z);
    double v5 = smoothNoise(integer_X, integer_Y, integer_Z + 1);
    double v6 = smoothNoise(integer_X + 1, integer_Y, integer_Z + 1);
    double v7 = smoothNoise(integer_X, integer_Y + 1, integer_Z + 1);
    double v8 = smoothNoise(integer_X + 1, integer_Y + 1, integer_Z + 1);

    double i1 = Interpolate(v1, v2, fractional_X);
    double i2 = Interpolate(v3, v4, fractional_X);
    double i3 = Interpolate(v5, v6, fractional_X);
    double i4 = Interpolate(v7, v8, fractional_X);

    double r1 = Interpolate(i1, i2, fractional_Y);
    double r2 = Interpolate(i3, i4, fractional_Y);

    return Interpolate(r1, r2, fractional_Z);
  }

  double Interpolate(double v1, double v2, double x) {
    return cosineInterpolate(v1, v2, x);
  }

  double smoothNoise(int x) {
    return noise(x) / 2 + (noise(x - 1) + noise(x + 1)) / 4;
  }

  double smoothNoise(int x, int y) {
    double corners = (noise(x - 1, y - 1) + noise(x + 1, y - 1) + noise(x - 1, y + 1) + noise(x + 1, y + 1)) / 16;
    double sides = (noise(x - 1, y) + noise(x + 1, y) + noise(x, y - 1) + noise(x, y + 1)) / 8;
    double center = noise(x, y) / 4;

    return corners + sides + center;
  }

  double smoothNoise(int x, int y, int z) {
    double corners = (noise(x - 1, y - 1, z) + noise(x + 1, y - 1, z) + noise(x - 1, y + 1, z) + noise(x + 1, y + 1, z)) / 16;
    double sides = (noise(x - 1, y, z) + noise(x + 1, y, z) + noise(x, y - 1, z) + noise(x, y + 1, z)) / 8;
    double center = noise(x, y, z) / 4;

    return corners + sides + center;
  }

  // untested version...
  //  double smoothNoise(int x, int y, int z) {
  //    double corners = (noise(x - 1, y - 1, z - 1) + noise(x + 1, y - 1, z - 1) + noise(x - 1, y + 1, z - 1) + noise(x + 1, y + 1, z - 1) +
  //            noise(x - 1, y - 1, z + 1) + noise(x + 1, y - 1, z + 1) + noise(x - 1, y + 1, z + 1) + noise(x + 1, y + 1, z + 1)) / 32;
  //    double sides = (noise(x - 1, y, z - 1) + noise(x + 1, y, z - 1) + noise(x, y - 1, z - 1) + noise(x, y + 1, z - 1) +
  //            noise(x - 1, y, z + 1) + noise(x + 1, y, z + 1) + noise(x, y - 1, z + 1) + noise(x, y + 1, z + 1)) / 16;
  //    double center = noise(x, y, z) / 6;
  //
  //    return corners + sides + center;
  //  }

  double noise(int n) {
    int x = (n << 13) ^ n;
    int a = (x * x * (int) prime[0]) + (int) prime[1];
    int b = x * a + (int) prime[2];
    if (b < 0) {
      b = -b;
    }
    return (double) (1.0 - (b / prime[3]));
  }

  double noise(int x, int y) {
    return noise(x + y * 57);
  }

  double noise(int x, int y, int z) {
    return noise(x + y * 57 + z * 57 * 57);
  }

};

}

#endif	/* PERLINNOISE_H */

