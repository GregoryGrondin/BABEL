/*
** EPITECH PROJECT, 2019
** NtekS
** File description:
** input.hpp
*/

#include "Encoder.hpp"

Encoder::Encoder()
{
    int err;

    encoder = opus_encoder_create(SAMPLE_RATE, 2, OPUS_APPLICATION_VOIP, &err);
    decoder = opus_decoder_create(SAMPLE_RATE, 2, NULL);
}

void print_float(int size, float *sampleBlock)
{
    int c = 0;
    while (c != size) {
        std::cout << sampleBlock[c] << " ";
        c++;
    }
}

int Encoder::encodeStream(float *sampleBlock, int numBytes, CSocket *socket)
{
    unsigned char *cbits = (unsigned char *)malloc(sizeof(unsigned char) * (FRAME_SIZE * 2));

    int nb = opus_encode_float(encoder, sampleBlock, FRAME_SIZE, cbits, numBytes);
    std::cout << "nb:" << nb << "\n" ;

    socket->sendData((float *)cbits, nb);
    return (1);
}

int Encoder::getLastSize()
{
    return (size);
}

float *Encoder::decodeStream(CSocket *socket)
{
    float *cbits = (float *)malloc(sizeof(float) * (FRAME_SIZE * 2));
    unsigned char *sampleBlock = (unsigned char *)socket->readData();
    int size1 = socket->getNbBytes() - 5;
    std::cout <<  "\n\n";
    std::cout << "size r:" << size1 << "\n";

    size = opus_decode_float(decoder, (sampleBlock), size1, cbits, FRAME_SIZE, 0);

    std::cout << size <<  "\n";

    return (cbits);
}