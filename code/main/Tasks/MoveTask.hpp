#pragma once

#include "BasicTask.hpp"

#define DISTANCE_THRESHOLD 2

class MoveTask : public BasicTask
{

private:
    PositionControler *positionControler;
    DriveControler *driveControler;
    ValueConverter *valueConverter;

    Point target_position;

    double distance_to_target_cm = 0;
    double distance_to_target_pulse = 0;

    void _Update() override
    {
        driveControler->SetDistance(distance_to_target_pulse);
        driveControler->SetAngle(0);
    }

    bool _IsDone() override
    {
        // check if the current position is close enough to the target position
        Point current_position = positionControler->GetCurrentPoint();

        double distance_to_target = sqrt(pow(target_position.x - current_position.x, 2) + pow(target_position.y - current_position.y, 2));

        return distance_to_target < DISTANCE_THRESHOLD;
    }

public:
    MoveTask(PositionControler *positionControler, DriveControler *driveControler, ValueConverter *valueConverter, Point target_position)
    {
        this->positionControler = positionControler;
        this->driveControler = driveControler;
        this->valueConverter = valueConverter;

        this->target_position = target_position;
    }

    void Compute() override
    {
        Point current_position = positionControler->GetCurrentPoint();

        distance_to_target_cm = sqrt(pow(target_position.x - current_position.x, 2) + pow(target_position.y - current_position.y, 2));

        distance_to_target_pulse = valueConverter->DistanceCMToPulse(distance_to_target_cm);
    }

    void Debug() override
    {
        Serial.print("Target_x:");
        Serial.print(target_position.x);
        Serial.print(",");
        Serial.print("Target_y:");
        Serial.print(target_position.y);
        Serial.print(",");

        Point current_position = positionControler->GetCurrentPoint();
        double error = sqrt(pow(target_position.x - current_position.x, 2) + pow(target_position.y - current_position.y, 2));

        Serial.print("Distance_to_travel_cm:");
        Serial.print(distance_to_target_cm);
        Serial.print(",");
        Serial.print("Distance_to_travel_pulse:");
        Serial.print(distance_to_target_pulse);
        Serial.print(",");
        Serial.print("Current_distance_pulse:");
        Serial.print(driveControler->GetDistance());
        Serial.print(",");

        Serial.print("Error_cm:");
        Serial.println(error);
    }
};