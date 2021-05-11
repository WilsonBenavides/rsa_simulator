/*
 * Utils.h
 *
 *  Created on: 9/04/2021
 *      Author: Wilson
 */

#ifndef NODE_UTILS_H_
#define NODE_UTILS_H_

#include <vector>
#include <string>
#include <omnetpp.h>

#define LIGHTPATH_REQUEST 0
#define LIGHTPATH_ROUTING 1
#define LIGHTPATH_ASSIGNMENT 2
#define MAXIMUM 1000
#define STR_LEN 7

using namespace omnetpp;

static cFigure::Color generateRandomColor()
{
    unsigned char str[STR_LEN + 1] = { 0 };
    const char *hex_digits = "0123456789ABCDEF";
    for (int i = 0; i < STR_LEN; i++) {
        str[i] = hex_digits[rand() % 15];
    }
    str[0] = '#';
    return cFigure::Color((char*) str);
}

#endif /* NODE_UTILS_H_ */
