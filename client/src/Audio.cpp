/*
** EPITECH PROJECT, 2019
** NtekS
** File description:
** input.hpp
*/


#include "Audio.hpp"
#include "Encoder.hpp"


Audio::Audio(CSocket *socket) : _socket(socket)
{
    PaStreamParameters inputParameters, outputParameters;

    Pa_Initialize();
    encoder = new Encoder();

    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);

    if (socket->hello() == 0) {
        exit(84);
    }
}

int Audio::startStream()
{
    //int numChannels = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->maxInputChannels < Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->maxOutputChannels ? Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->maxInputChannels : Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->maxOutputChannels;
    int numChannels = 2;
    numBytes = FRAMES_PER_BUFFER * numChannels * 4;
    sampleBlock = (float *) malloc(numBytes);

    memset(sampleBlock, 0.0f, numBytes);

    Pa_StartStream(stream);
    return (0);
}

int Audio::readStream()
{
    // if (_socket->_role == CSocket::MASTER) {
    //   for (int i=0; i < (1 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
    //       Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER);
    //       encoder->encodeStream(sampleBlock, numBytes, _socket);
    //   }
    // } else {
    //   for (int i=0; i < (1 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
    //       Pa_WriteStream(stream, encoder->decodeStream(_socket), encoder->getLastSize());
    //   }
    // }

    for (int i=0; i < (1 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER);
        encoder->encodeStream(sampleBlock, numBytes, _socket);
        Pa_WriteStream(stream, encoder->decodeStream(_socket), FRAMES_PER_BUFFER);
    }   

    return (0);
}

int Audio::readStreamWithoutEncoding(int hangout)
{
    // if (_socket->_role == CSocket::MASTER) {
    //   for (int i=0; i < (1 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
    //       Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER);
    //       _socket->sendData((float *)sampleBlock, numBytes);
    //   }
    // } else {
    //   for (int i=0; i < (1 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
    //       Pa_WriteStream(stream, _socket->readData(), FRAMES_PER_BUFFER);
    //   }
    // }
    if (hangout == 1) {
        _socket->sendData((float *)NULL, numBytes);
        return (1);
    }
    float *a;

    for (int i=0; i < (1 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER);
        _socket->sendData((float *)sampleBlock, numBytes);
        if ((a = _socket->readData()) == NULL) {
            return (1);
        }
        Pa_WriteStream(stream, a, FRAMES_PER_BUFFER);
    }

    return (0);
}

int Audio::stopStream()
{
    Pa_StopStream(stream);
    Pa_Terminate();
    return (0);
}
