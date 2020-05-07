import 'package:flutter/material.dart';
import 'dart:math';

class IndicatoreOzono extends CustomPainter{

  double angle;
  double startAngle;
  double finishAngle;
  Color colore;

  double valoreMassimo;

  IndicatoreOzono.base(){
    this.angle = 0.5 * 270;
    this.startAngle = inRadiant(135);
    this.finishAngle = inRadiant(270);

    this.valoreMassimo = 270;
  }
  //_indicare anche massimo e minimo
  IndicatoreOzono(double valore, double valoreMassimo, Color colore){
    this.valoreMassimo = valoreMassimo;
  
    this.startAngle = inRadiant(135) ;
    this.finishAngle = inRadiant(270);

    this.angle = (valore / this.valoreMassimo) * 270;

    this.colore = colore;
  }

  @override
  void paint(Canvas canvas, Size size) {

    Paint arc = Paint()
    ..color = this.colore
    ..style = PaintingStyle.fill;

    Paint circle = Paint()
    ..color = Colors.white
    ..style = PaintingStyle.fill;

    Offset center = Offset(size.width /2, size.height /2);
    double radius = min(size.width/2, size.height/2);

    Path arco = Path()
    // set the "current point"
    ..moveTo(center.dx, center.dy)
    ..arcTo(Rect.fromCircle(center: center, radius: radius), this.startAngle, inRadiant(this.angle), false);

    Path cerchio = Path()
    ..addOval(Rect.fromCircle(center: Offset(size.width/2 - 5, size.height/2 + 5), radius: radius - 35));

    canvas.drawPath(arco, arc);
    canvas.drawShadow(cerchio, Colors.black54, 7, true);
    canvas.drawCircle(center, radius - 30, circle);
    
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) {
    return true;
  }

}


double inRadiant(double angle){
  double radiant = (angle / 270.0) * ( (3/2) * pi);

  return radiant;
}