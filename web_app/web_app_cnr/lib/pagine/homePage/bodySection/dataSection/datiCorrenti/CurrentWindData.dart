import 'package:flutter/material.dart';
import '../indicatori/WindIndicator.dart';
import 'dart:math';

import '../../../HomePage.dart' show Data, WindValue;

///create and return all the wind indicator section 
///including the part with all the neccessary information 
///such as:
///1. the value
///2. the unit of measurment
///3. the date of that value
///4. the two button to move around the chart
class CurrentWindData extends StatefulWidget {

  ///the unit of measurement of the indicator
  String _unit;

  ///the strenght and the direction that the indicator will show
  WindValue _data;

  ///the date that the indicator will show
  DateTime _date;

  ///the color of the indicator
  Color _color;

  ///the width of the indicator
  double _width;

  ///the height of the indicator
  double _height;

  ///the callBack function that permit to change the showed data
  Function _selectData;
  

  ///create and return all the wind indicator section 
  ///including the part with all the neccessary information 
  ///such as:
  ///1. the value
  ///2. the unit of measurment
  ///3. the date of that value
  ///4. the two button to move around the chart
  CurrentWindData(String unit, WindValue data, DateTime date, Color color, double width, double height, {Function selectData}){
    this._unit = unit;
    this._data = data;
    this._date = date;

    this._color = color;
    this._width = width;
    this._height = height;
    this._selectData = selectData;


    //this._index = index;
    //this._finalValue = this._data[this._index].value;
  }

  @override
  _CurrentWindDataState createState() => _CurrentWindDataState();
}


class _CurrentWindDataState extends State<CurrentWindData> with SingleTickerProviderStateMixin{
  ///the animation controller for the wind strenght
  AnimationController controllerStrenght;

  ///the animation controller for the wind direction
  AnimationController controllerDirection;

  ///the animation for the wind strenght
  Animation<double> strenght;

  ///the animation for the wind direction
  Animation<double> direction;
  
  ///initialize: the [controllerStrenght] and [controllerDirection]
  //7in addition start for the first time the animation
  @override
  void initState() {
    controllerStrenght = AnimationController(vsync: this, duration: Duration(seconds: 1));
    controllerStrenght.addListener(() {
      setState(() {});
    });
    strenght = Tween<double>(begin: 0, end: widget._data.strenght).chain(CurveTween(curve: Curves.easeOutQuad)).animate(controllerStrenght);

    

    controllerDirection = AnimationController(vsync: this, duration: Duration(milliseconds: 1000));
    controllerDirection.addListener(() {
      setState(() {});
    });
    direction = Tween<double>(begin: 0, end: widget._data.direction ).chain(CurveTween(curve: Curves.easeOutQuad)).animate(controllerDirection);

    controllerDirection.forward();
    controllerStrenght.forward();

    super.initState();
  }

  @override
  void dispose() {
    controllerDirection.dispose();
    controllerStrenght.dispose();
    super.dispose();
  }

  ///trasform the degrees in radiant
  double inRadiant(double angle){
    double radiant = (angle / 360) * ( 2 * pi);

    return radiant;
  }

  ///invole the callback [widget._selectData] function and pass it -1 to select the previous data
  ///[widget._selectData] need also [updateAnimation] to recreates the animations and restarts it
  void goBack(){
    
    widget._selectData( -1 , updateAnimation);
  }

  ///invole the callback [_selectData] function and pass it +1 to select the following data
  ///[widget._selectData] need also [updateAnimation] to recreates the animations and restarts it
  void goForward(){
    
    widget._selectData( 1 , updateAnimation);

  }

  ///is called when indicator is updated, recreates the controllers and and the animations and restarts them
  void updateAnimation(WindValue begin, WindValue end){
        
    controllerStrenght.dispose();
    controllerDirection.dispose();

    controllerStrenght = AnimationController(vsync: this, duration: Duration(seconds: 1));
    controllerStrenght.addListener(() {
      setState(() {});
    });
    strenght = Tween<double>(begin: begin.strenght, end: end.strenght).chain(CurveTween(curve: Curves.easeOutQuad)).animate(controllerStrenght);

    
    controllerDirection = AnimationController(vsync: this, duration: Duration(milliseconds: 1000));
    controllerDirection.addListener(() {
      setState(() {});
    });
    direction = Tween<double>(begin: begin.direction, end: end.direction ).chain(CurveTween(curve: Curves.easeOutQuad)).animate(controllerDirection);

    controllerDirection.forward();
    controllerStrenght.forward();
  }

  
  
  @override
  Widget build(BuildContext context) {
    //print(widget._data.direction);

    return Container(
        child: Column(
          children: <Widget>[
            Expanded(
              flex: 8,
              child: Stack(
                alignment: Alignment.center,
                children: [
                  //_ animation
                  AnimatedBuilder(
                    animation: controllerDirection,
                    builder: (context, child) {
                      return Transform.rotate(angle: inRadiant(direction.value), child: child);
                    },

                    //_indicator
                    child: CustomPaint(

                      painter: WindIndicator(direction.value, strenght.value, 100, 0, widget._color),
                      size: Size(widget._width, widget._height),

                      
                    ),
                  
                  ),

                  Container(
                    width: widget._width,
                    height: widget._height,

                    child: Center(//_inner part of the indicator 
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: <Widget>[
                          Text(
                            "${widget._date.toString().split(" ")[0]} value",

                            style: TextStyle(fontSize: 18),
                          ),

                          Column(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: <Widget>[
                              Text(//_ part that show the value
                                "${strenght.value.toStringAsFixed(1)}",
                                style: TextStyle(fontSize: 50),
                              ),

                              Text(//_ unit of measurement
                                widget._unit,
                                style: TextStyle(fontSize: 25, fontWeight: FontWeight.w500),
                              )
                            ],
                          ),

                        ],
                      ),
                    ),
                  ),

                ],
              ),
            ),
            
            

            Expanded(
              flex: 2,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: <Widget>[

                  FlatButton( //_ goBack button
                    onPressed: () { goBack(); },
                    onLongPress: () => null,
                    child: Icon(Icons.arrow_back_ios),

                    splashColor: Colors.lightBlue,
                    highlightColor: Colors.lightBlue,
                    hoverColor: Colors.lightBlue[100],

                  ),
                  FlatButton( //_ goForward button 
                    onPressed: () { goForward(); },
                    onLongPress: () => null,
                    child: Icon(Icons.arrow_forward_ios),

                    splashColor: Colors.lightBlue,
                    highlightColor: Colors.lightBlue,
                    hoverColor: Colors.lightBlue[100],

                  )

                ],
              )
            )


          ],
        ),
      );
  }
}