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
#include <string.h>
#include "CSocket.hpp"
#include "Encoder.hpp"

class IAudio
{
        public:
                virtual ~IAudio() = default;
        protected:
                Encoder *encoder;
};
