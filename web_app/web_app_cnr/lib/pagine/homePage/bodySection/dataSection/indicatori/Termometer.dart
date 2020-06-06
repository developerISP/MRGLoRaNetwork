import 'package:flutter/material.dart';
import 'dart:math';

class Termometer extends CustomPainter{

  ///the color of the termometer
  Color color;
  
  ///the maximun value that the termometer can reach
  double maxValue;

  ///the height of the "mercury" in the indicator 
  double heightOfMercury;

  ///the radius of the ball at the bottom of the termometer (inner part) 
  double inRadius = 35;

  ///the radius of the ball at the bottom of the termometer (external part) 
  double exRadius = 50;
  
  ///generate a termometer with the given parameter
  ///
  ///the parameter are:
  ///1. `value`: decide how much the mercury column have to rise
  ///2. `maxValue`: define the maximum value that the termometer can show
  ///3. `color`: define the color of the termometer
  ///
  ///in addiction, if you pass a negative value,
  ///it will automatically calculate the module of that number
  ///
  ///is checked also if the value overtake the maxValue and in that case
  ///the value will be set to the maximum
  Termometer(double value, double maxValue, Color color){
    this.color = color;
    this.maxValue = maxValue;

    this.heightOfMercury = mod(value) <= maxValue 
    ? (value / this.maxValue)
    : 1;
  }

  @override
  void paint(Canvas canvas, Size size) {
    int moveToLeft = -50;

    //_ set the center of the canvas
    Offset center = Offset(size.width/2 +moveToLeft, size.height/2);

    //_ set the center of the ball of the termometer
    Offset ballCenter = Offset(size.width/2 +moveToLeft, size.height- ( exRadius- ((exRadius-inRadius)/2) ) );

    //_ set the center of the shadow
    Offset shadowCenter = Offset(size.width/2 -5 +moveToLeft, size.height-inRadius +5);

    //_ set the maximum height for the mercury column
    double maxHeight = size.height -((exRadius-inRadius)) -(inRadius*2) -2.5;

    //_ define the style of the mercury
    Paint mercury = Paint()
    ..color = this.color
    ..style = PaintingStyle.fill;

    //_ define the style of the column
    Paint column = Paint()
    ..color = Colors.white
    ..style = PaintingStyle.fill;

    //_ set the path pf the external part
    Path externalPart = Path()
    ..moveTo(center.dx, center.dy)
    ..addRRect(RRect.fromLTRBR(ballCenter.dx-exRadius/2, 0, ballCenter.dx+exRadius/2, ballCenter.dy, Radius.circular(30)))
    ..addOval(Rect.fromCircle(center: ballCenter, radius: exRadius-((exRadius-inRadius)/2) ) );

    //_ set the path of the inner part
    Path innerPart = Path()
    ..moveTo(center.dx, center.dy)
    ..addRRect(RRect.fromLTRBR(ballCenter.dx-inRadius/2, maxHeight*(1-this.heightOfMercury) + ((exRadius-inRadius)/2), ballCenter.dx+inRadius/2, ballCenter.dy, Radius.circular(30)))
    ..addOval(Rect.fromCircle(center: ballCenter, radius: inRadius));
    //..addRect(Rect.fromCenter(center: centro, height: 100, width: 50));

    //_ set the path of the shadow
    Path shadow = Path()
    ..moveTo(center.dx, center.dy)
    ..addRRect(RRect.fromLTRBR(shadowCenter.dx-exRadius/2, 5, shadowCenter.dx+exRadius/2, shadowCenter.dy, Radius.circular(30)))
    ..addOval(Rect.fromCircle(center: shadowCenter, radius: (exRadius-(exRadius-inRadius)/2)-5 ));

    //_ render everything
    canvas.drawShadow(shadow, Colors.black54, 7, true);
    canvas.drawPath(externalPart, column);
    canvas.drawPath(innerPart, mercury);

  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return true;
  }

  ///return the module of n
  double mod(double n){
    return n >= 0
    ? n
    : n*(-1);
  }

}