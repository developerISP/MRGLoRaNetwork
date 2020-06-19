import 'package:flutter/material.dart';
import "dart:math";

///Draw the ozon indicator and parse the value to trasform it in the pointer angle
class OzonIndicator extends CustomPainter{

  ///indicate the angle from the [startAngle] to the angle of the pointer
  double angle;

  ///indicate the starting angle of the indicator 
  ///
  /// **it corrispond to the point where the pointer indicate the zero**
  double startAngle;

  ///indicate the finishing angle of the indicator
  ///
  /// **it correspond to the point where the pointer indicate the maximum value**
  double finishAngle;

  ///the color of the indicator
  Color color;

  ///the maximum value of the indicator
  double maxValue;

  ///the minimun value of the indicator
  double minValue;

  ///generate a basis indicator
  ///
  ///* the angle of the pointer is half of the maximum angle
  ///* the [startAngle] is 135°
  ///* the [finishAngle] is 270°
  ///* the [maxValue] is 270°
  ///
  OzonIndicator.base(){
    this.angle = 0.5 * 270;
    this.startAngle = inRadiant(135);
    this.finishAngle = inRadiant(270);

    this.maxValue = 270;
  }

  ///generate the ozon indicator with the given parameters
  ///
  ///the parameters are:
  ///* `value` the value that you want to show
  ///* `maxValue` the maximum value that the indicator can reach
  ///* `color` the color of the indicator
  ///
  ///in addiction, if you pass a negative value,
  ///it will automatically calculate the module of that number
  ///
  ///is checked also if the value overtake the maxValue and in that case
  ///the value will be set to the maximum
  OzonIndicator(double value, double maxValue, double minValue, Color color){
    this.maxValue = maxValue;
    this.minValue = minValue;
  
    this.startAngle = inRadiant(135) ;
    this.finishAngle = inRadiant(270);



    this.angle = value <= this.maxValue && value >= this.minValue
    ? ( (mod(this.minValue) + value) / (this.maxValue + mod(this.minValue)) ) * 270
    : value < this.minValue 
      ? 1
      : 270;

    this.color = color;
  }

  @override
  void paint(Canvas canvas, Size size) {
    
    //_ define the style of the arc blue
    Paint arcStyle = Paint()
    ..color = this.color
    ..style = PaintingStyle.fill;

    //_ define the style of inner white circle
    Paint circleStyle = Paint()
    ..color = Colors.white
    ..style = PaintingStyle.fill;

    Offset center = Offset(size.width /2, size.height /2);
    double radius = min(size.width/2, size.height/2);

    //_ draw the blue arc
    Path arc = Path()
    ..moveTo(center.dx, center.dy)
    ..arcTo(Rect.fromCircle(center: center, radius: radius), this.startAngle, inRadiant(this.angle), false);

    //_ draw the inner white circle
    Path cirlce = Path()
    ..addOval(Rect.fromCircle(center: Offset(size.width/2 - 5, size.height/2 + 5), radius: radius - 35));

    //_ render everything
    canvas.drawPath(arc, arcStyle);

    canvas.drawShadow(cirlce, Colors.black, 5, true);
    canvas.drawCircle(center, radius - 30, circleStyle);
    
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return true;
  }

  ///trasform the degrees in radiant
  double inRadiant(double angle){
    double radiant = (angle / 360) * ( 2 * pi);

    return radiant;
  }

  ///return the module of n
  double mod(double n){
    return n >= 0
    ? n
    : n*(-1);
  }

}


