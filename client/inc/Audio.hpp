/*
** EPITECH PROJECT, 2019
** NtekS
** File description:
** IComponent.hpp
*/

#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <math.h>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <opus/opus.h>
#include "CSocket.hpp"
#include "IAudio.hpp"
#include "main.hpp"

class Audio: public IAudio
{
        public:
                Audio(CSocket *socket);
                void Setup();
                int startStream();
                int readStream();
                int readStreamWithoutEncoding(int);
                int stopStream();
        protected:
                CSocket *_socket;
                PaStream* stream;
                int numBytes;
                float *sampleBlock;
};