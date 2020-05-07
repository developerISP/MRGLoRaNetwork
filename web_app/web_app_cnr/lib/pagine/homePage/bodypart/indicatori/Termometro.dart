
import 'package:flutter/material.dart';
import 'dart:math';

class Termometro extends CustomPainter{

  Color colore;
  double valoreMassimo;
  double valorePerc;


  double radius = 35;
  double colRadius = 50;
  

  Termometro(valore, valoreMassimo, colore){
    this.colore = colore;
    this.valoreMassimo = valoreMassimo;

    this.valorePerc = (valore / this.valoreMassimo);
  }

  @override
  void paint(Canvas canvas, Size size) {

    Offset centro = Offset(size.width/2 -50, size.height/2);
    Offset nucleo = Offset(size.width/2 -50, size.height- ( colRadius- ((colRadius-radius)/2) ) );
    Offset centroOmbra = Offset(size.width/2 -5 -50, size.height-radius +5);

    double altezzaMassima = size.height -((colRadius-radius)) -(radius*2) -2.5;

    Paint mercurio = Paint()
    ..color = this.colore
    ..style = PaintingStyle.fill;

    Paint colonnina = Paint()
    ..color = Colors.white
    ..style = PaintingStyle.fill;

    Path esterno = Path()
    ..moveTo(centro.dx, centro.dy)
    ..addRRect(RRect.fromLTRBR(nucleo.dx-colRadius/2, 0, nucleo.dx+colRadius/2, nucleo.dy, Radius.circular(30)))
    ..addOval(Rect.fromCircle(center: nucleo, radius: colRadius-((colRadius-radius)/2) ) );

    Path interno = Path()
    ..moveTo(centro.dx, centro.dy)
    ..addRRect(RRect.fromLTRBR(nucleo.dx-radius/2, altezzaMassima*(1-this.valorePerc) + ((colRadius-radius)/2), nucleo.dx+radius/2, nucleo.dy, Radius.circular(30)))
    ..addOval(Rect.fromCircle(center: nucleo, radius: radius));
    //..addRect(Rect.fromCenter(center: centro, height: 100, width: 50));

    Path ombra = Path()
    ..moveTo(centro.dx, centro.dy)
    ..addRRect(RRect.fromLTRBR(centroOmbra.dx-colRadius/2, 5, centroOmbra.dx+colRadius/2, centroOmbra.dy, Radius.circular(30)))
    ..addOval(Rect.fromCircle(center: centroOmbra, radius: (colRadius-(colRadius-radius)/2)-5 ));

    canvas.drawShadow(ombra, Colors.black54, 7, true);
    canvas.drawPath(esterno, colonnina);
    canvas.drawPath(interno, mercurio);

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