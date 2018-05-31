
// Fill out your copyright notice in the Description page of Project Settings.

#include "Perlin.h"

int Perlin::mask = 255;

int Perlin::size = mask + 1;

std::array<uint_fast8_t, 512> Perlin::perm = InitPerm();

float Perlin::Fade(const float t)
{
	return pow(t, 3) * (t * (t * 6 - 15) + 10);
}

float Perlin::Lerp(const float t, const float a, const float b)
{
	return a + t * (b - a);
}

float Perlin::Grad(const int hash, const float x, const float y, const float z)
{
	const auto h = hash & 15;
	const auto u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

void Perlin::Reseed(const int seed)
{
	perm = InitPerm();
	std::shuffle(perm.begin(), perm.begin() + size, std::default_random_engine(seed));
	for (size_t i = 0; i < size; ++i)
	{
		perm[size + i] = perm[i];
	}
}

float Perlin::Noise(const float x, const float y)
{
	return Noise(x, y, 0.0f);
}

float Perlin::Noise(const float x, const float y, const float z)
{
	const auto unit_x = (int)floor(x) & 255,
		unit_y = (int)floor(y) & 255,
		unit_z = (int)floor(z) & 255;
	const auto sub_x = x - floor(x),
		sub_y = y - floor(y),
		sub_z = z - floor(z);
	const auto u = Fade(sub_x),
		v = Fade(sub_y),
		w = Fade(sub_z);
	const auto a = perm[unit_x] + unit_y,
		aa = perm[a] + unit_z,
		ab = perm[a + 1] + unit_z,
		b = perm[unit_x + 1] + unit_y,
		ba = perm[b] + unit_z,
		bb = perm[b + 1] + unit_z;
	return Lerp(w,
		Lerp(
			v,
			Lerp(
				u,
				Grad(
					perm[aa],
					sub_x,
					sub_y,
					sub_z),
				Grad(
					perm[ba],
					sub_x - 1,
					sub_y,
					sub_z)),
			Lerp(
				u,
				Grad(
					perm[ab],
					sub_x,
					sub_y - 1,
					sub_z),
				Grad(
					perm[bb],
					sub_x - 1,
					sub_y - 1,
					sub_z))),
		Lerp(
			v,
			Lerp(
				u,
				Grad(
					perm[aa + 1],
					sub_x,
					sub_y,
					sub_z - 1),
				Grad(
					perm[ba + 1],
					sub_x - 1,
					sub_y,
					sub_z - 1)),
			Lerp(
				u,
				Grad(
					perm[ab + 1],
					sub_x,
					sub_y - 1,
					sub_z - 1),
				Grad(
					perm[bb + 1],
					sub_x - 1,
					sub_y - 1,
					sub_z - 1))));
}

float Perlin::Noise(float x, float y, const int octaves)
{
	return Noise(x, y, 0.0f, octaves);
}

float Perlin::Noise(float x, float y, float z, const int octaves)
{
	float result = 0.0f;
	float amp = 1.0f;
	for (int i = 0; i < octaves; ++i)
	{
		result += Noise(x, y, z) * amp;
		x *= 2.0f;
		y *= 2.0f;
		z *= 2.0f;
		amp *= 0.5f;
	}
	return result;
}
