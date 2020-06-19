import 'dart:ui';

import 'package:flutter/material.dart';
import 'dart:math' as math;

import 'dart:ui' as ui;

///Draw the umidity indicator and parse the value to trasform it "filling" of the waterd drop
class UmidityIndicator extends CustomPainter {

  ///the color of the water drop
  Color color;

  ///the maximun value that the indicator can reach
  double maxValue;

  ///the minimum value that the indicator can reach
  double minValue;

  ///that value that determine the filling of the water drop
  double value;

  
  /// generate a umidity indicator with the given parameters
  /// 
  ///the parameters are:
  ///* `value` the value that you want to show
  ///* `maxValue` the maximum value that the indicator can reach
  ///* `minValue` the minimun value that the indicator can reach
  ///* `color` the color of the indicator
  ///
  ///in addiction, if you pass a negative value,
  ///it will automatically calculate the module of that number
  ///
  ///is checked also if the value overtake the maxValue and in that case
  ///the value will be set to the maximum
  UmidityIndicator(double value, double maxValue, double minvalue,  Color color){
    this.color = color;
    this.maxValue = maxValue;
    this.minValue = minvalue;

    this.value = value >= minValue && value <= maxValue 
    ? (mod(this.minValue) + value) / (mod(this.minValue) + this.maxValue)
    : value < this.minValue
      ? 0.01
      : 1;
  }

  @override
  void paint(Canvas canvas, Size size) {

    //_define how much the indicator have to move to the right
    int movetoright = 80;

    //_ define how much big the waterDrop has to be
    double waterDropRadius = 60;

    //_define the angle of the cone
    double coneAngle = inRadiant(30);
    
    //_define the height of the cone
    double coneHeight = math.sqrt( math.pow(waterDropRadius, 2) + math.pow( math.tan((math.pi/2)-coneAngle) , 2) );

    //_ define the center of the sphere that compose the water drop
    Offset circleCenter = Offset(size.width/2 + movetoright, (size.height/2) + waterDropRadius);

    //_ define the style of the waterdrop
    Paint waterDropStyle = Paint()
    ..color = this.color
    ..style = PaintingStyle.fill;

    //_ define the style of the external border of the water drop
    Paint waterDropExternalStyle = Paint()
    ..color = Colors.white
    ..style = PaintingStyle.fill;

    //_ define all the water drop part that compose the indicator
    //_ the inner part of the water drop, the external border, the shadow
    Path waterDrop = drawWaterDrop(waterDropRadius, circleCenter);
    Path waterDropBorder = drawWaterDrop(waterDropRadius + 10, Offset(circleCenter.dx, circleCenter.dy));
    Path waterDropShadow = drawWaterDrop(waterDropRadius + 10, Offset(circleCenter.dx -5, (circleCenter.dy -10) +5 ));
    Path waterDropFill = Path()
    //top left point
    ..moveTo(circleCenter.dx -waterDropRadius,   (circleCenter.dy + waterDropRadius) - ( (waterDropRadius*2) + coneHeight ) * this.value)
    //top right point
    ..lineTo(circleCenter.dx +waterDropRadius,   (circleCenter.dy + waterDropRadius) - ( (waterDropRadius*2) + coneHeight ) * this.value)
    //bottom right point
    ..lineTo(circleCenter.dx +waterDropRadius,   circleCenter.dy +waterDropRadius)
    //bottom left point
    ..lineTo(circleCenter.dx -waterDropRadius,    circleCenter.dy +waterDropRadius)
    ..close();
    
    //_ render everything
    canvas.drawShadow(waterDropShadow, Colors.black, 5, true);
    canvas.drawPath(waterDropBorder, waterDropExternalStyle);
    canvas.clipPath(waterDrop);

    canvas.drawPath(waterDropFill, waterDropStyle);

  }

  ///draw a water drop with the given `radius` and Offset
  ///
  ///`radius` means the radius of the spheric botton part of the water drop
  Path drawWaterDrop(double waterDropRadius, Offset circleCenter){

    //_ define the angle where the conic part should start
    double coneAngle = inRadiant(30);
    //_ define the height of the conic part
    double coneHeight = math.sqrt( math.pow(waterDropRadius, 2) + math.pow( math.tan((math.pi/2)-coneAngle) , 2) );
    

    //_ define the top point of the waterdrop
    math.Point topPoint = math.Point(circleCenter.dx, circleCenter.dy-coneHeight - waterDropRadius);

    //_ define the side point where the conic part start
    var x =  math.cos( coneAngle ) * waterDropRadius ;
    var y =  math.sin( coneAngle ) * waterDropRadius ;
    math.Point sidePoint = math.Point(x, y);

    //_ define the path of the spheric part
    Path waterDrop = Path()
    ..moveTo(circleCenter.dx, circleCenter.dy)
    ..arcTo(Rect.fromCircle(center: Offset(circleCenter.dx, circleCenter.dy), radius: waterDropRadius), inRadiant(-30), inRadiant(240), false)
    ..lineTo(topPoint.x, topPoint.y)
    ..lineTo(circleCenter.dx + sidePoint.x, circleCenter.dy - sidePoint.y);
    

    return waterDrop;

  }



  @override
  bool shouldRepaint(UmidityIndicator oldDelegate) => false;

  @override
  bool shouldRebuildSemantics(UmidityIndicator oldDelegate) => false;

  ///return the module of n
  double mod(double n){
    return n >= 0
    ? n
    : n*(-1);
  }

  ///trasform the degrees in radiant
  double inRadiant(double angle){
    double radiant = (angle / 360) * ( 2 * math.pi);

    return radiant;
  }
}