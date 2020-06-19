import 'dart:math';
import 'dart:ui' as ui;

import 'package:flutter/material.dart';

class WindIndicator extends CustomPainter {

  /// the strenght of the wind
  double strenght;

  /// the direction of the wind
  /// 
  /// **0** = north
  /// 
  /// **pi/2** = west
  /// 
  /// **pi** = south
  /// 
  /// **2/3*pi** = est
  double direction;

  ///maximum wind strenght that the indicator can show
  double maxStrenght;

  ///minimum wind strenght that the indicator can show
  double minStrenght;

  ///the color of the indicator
  Color color;

  WindIndicator(double direction, double strenght, double maxStrenght, double minStrenght,  Color color){
    
    this.color = color;
    this.maxStrenght = maxStrenght;
    this.minStrenght = minStrenght;

    this.strenght = strenght >= minStrenght && strenght <= maxStrenght 
    ? (mod(minStrenght) + strenght) / (mod(minStrenght) + this.maxStrenght)
    : strenght < minStrenght
      ? 0
      : 1;

    this.direction = direction;
  }



  @override
  void paint(Canvas canvas, Size size) {
    //_ define all cardinal points
    ui.Paragraph north = createParagraph("N");
    ui.Paragraph south = createParagraph("S");
    ui.Paragraph est = createParagraph("E");
    ui.Paragraph west = createParagraph("W");

    //_ define the length of the arrow
    double arrowLength = 40;

    //_ define the radius of the inner circle
    double circleRadius = 90;

    //_ define the center of the canvas
    Offset center = Offset(0, 0);

    //_ define the style of the arrow
    Paint arrowStyle = Paint()
    ..color = this.color
    ..style = PaintingStyle.fill;

    //_ define the style of the inner circle
    Paint circleStyle = Paint()
    ..color = Colors.white
    ..style = PaintingStyle.fill;

    //_ define the path of the arrow
    Path arrow = Path()
    ..moveTo(-10, 0)
    ..lineTo(-10, circleRadius + (arrowLength * this.strenght) )
    ..lineTo(-20, circleRadius + (arrowLength * this.strenght))
    ..lineTo(0, circleRadius+30 + (arrowLength * this.strenght))
    ..lineTo(20, circleRadius + (arrowLength * this.strenght))
    ..lineTo(10, circleRadius + (arrowLength * this.strenght))
    ..lineTo(10, 0);

    //_ define the shadow of the inner circle 
    Path circleShadow = Path()
    ..addOval(Rect.fromCircle(center: Offset(center.dx-5, center.dy+ 5), radius: circleRadius));


    
    canvas.translate(size.width/2, size.height/2);

    canvas.drawShadow(circleShadow, Colors.black, 5, true);

    //_ rotate the arrow into the direction
    //+_ add pi so in this case the north will point up

    canvas.drawPath(arrow, arrowStyle);
    canvas.drawCircle(center, circleRadius, circleStyle);
    
    
    canvas.rotate(inRadiant(- (this.direction) ));

    canvas.drawParagraph(north, Offset(0, -(size.width/2)-15)        );
    canvas.drawParagraph(south, Offset(0, size.height/2)            );
    canvas.drawParagraph(est,   Offset((size.width/2)-15, 0)        );
    canvas.drawParagraph(west,  Offset(-(size.width/2), 0)          );
    

  }

  @override
  bool shouldRepaint(WindIndicator oldDelegate) => false;

  @override
  bool shouldRebuildSemantics(WindIndicator oldDelegate) => false;

  ///return the module of n
  double mod(double n){
    return n >= 0
    ? n
    : n*(-1);
  }

  ///trasform the degrees in radiant
  double inRadiant(double angle){
    double radiant = (angle / 360) * ( 2 * pi);

    return radiant;
  }

  ///create the label to indentify the direction of the arrow
  ui.Paragraph createParagraph(String string){

    ui.ParagraphBuilder paragraphBuilder = ui.ParagraphBuilder(
        ui.ParagraphStyle(
          fontSize:  15, 
          fontStyle:  FontStyle.italic,
          fontWeight: FontWeight.bold,
          textAlign: TextAlign.justify,
        )
      )
      ..pushStyle(ui.TextStyle(color: Colors.black))
      ..addText(string);

    final ui.Paragraph paragraph = paragraphBuilder.build()
      ..layout(ui.ParagraphConstraints(width: 100)); 

    return paragraph;
  }
}