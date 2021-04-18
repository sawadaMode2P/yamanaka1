/*
    Plotter.cpp

    Copyright © 2021 Wataru Taniguchi. All rights reserved.
*/
#include "Plotter.hpp"

Plotter::Plotter(ev3api::Motor* lm, ev3api::Motor* rm, ev3api::GyroSensor* gs) :
distance(0.0),azimuth(0.0),locX(0.0),locY(0.0),leftMotor(lm),rightMotor(rm),gyroSensor(gs) {
    /* reset motor encoders */
    leftMotor->reset();
    rightMotor->reset();
    /* reset gyro sensor */
    gyroSensor->reset();
    /* initialize variables */
    prevAngL = leftMotor->getCount();
    prevAngR = rightMotor->getCount();
}

int32_t Plotter::getDistance() {
    return (int32_t)distance;
}

int16_t Plotter::getAzimuth() {
    return (int32_t)azimuth;
}

int16_t Plotter::getDegree() {
    // degree = 360.0 * radian / M_TWOPI;
    int16_t degree = (360.0 * azimuth / M_TWOPI);
    if (degree > 360){
        degree -= 360;
    }
    return degree;
}

int32_t Plotter::getLocX() {
    return (int32_t)locX;
}

int32_t Plotter::getLocY() {
    return (int32_t)locY;
}

int32_t Plotter::getDeltaAngL() {
    return deltaAngL;
}

int32_t Plotter::getDeltaAngR() {
    return deltaAngR;
}

void Plotter::plot() {
    /* accumulate distance */
    int32_t curAngL = leftMotor->getCount();
    int32_t curAngR = rightMotor->getCount();
    deltaAngL = curAngL - prevAngL;
    if (deltaAngL > 180) {
        deltaAngL -= 360;
    } else if (deltaAngL < -180) {
        deltaAngL += 360;
    }
    deltaAngR = curAngR - prevAngR;
    if (deltaAngR > 180) {
        deltaAngR -= 360;
    } else if (deltaAngR < -180) {
        deltaAngR += 360;
    }
    double deltaDistL = M_PI * TIRE_DIAMETER * deltaAngL / 360.0;
    double deltaDistR = M_PI * TIRE_DIAMETER * deltaAngR / 360.0;
    double deltaDist = (deltaDistL + deltaDistR) / 2.0;
    distance += deltaDist;
    prevAngL = curAngL;
    prevAngR = curAngR;
    /* calculate azimuth */
    double deltaAzi = atan2((deltaDistL - deltaDistR), WHEEL_TREAD);
    azimuth += deltaAzi;
    if (azimuth > M_TWOPI) {
        azimuth -= M_TWOPI;
    } else if (azimuth < 0.0) {
        azimuth += M_TWOPI;
    }
    /* estimate location */
    locX += (deltaDist * sin(azimuth));
    locY += (deltaDist * cos(azimuth));
}