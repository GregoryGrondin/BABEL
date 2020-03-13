/*
** EPITECH PROJECT, 2019
** NtekS
** File description:
** IComponent.hpp
*/

#pragma once

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include <boost/asio.hpp>
#include <math.h>

#include <portaudio.h>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

#define SAMPLE_RATE 12000
#define FRAMES_PER_BUFFER 960
#define FRAME_SIZE 960