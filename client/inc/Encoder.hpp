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
#include "main.hpp"
#include "IEncoder.hpp"

class Encoder: public IEncoder
{
        public:
                Encoder();
                int encodeStream(float *, int, CSocket *);
                float *decodeStream(CSocket *);
                int getLastSize();
        protected:
                OpusEncoder *encoder;
                OpusDecoder *decoder;
                int size;
};