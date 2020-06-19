import 'package:flutter/material.dart';
import '../indicatori/Termometer.dart';


import '../../../HomePage.dart' show Data;

///create and return all the temperature indicator section 
///including the part with all the neccessary information 
///such as:
///1. the value
///2. the unit of measurment
///3. the date of that value
///4. the two button to move around the chart
class CurrentTempData extends StatefulWidget {

  ///the unit of measurement of the indicator
  String _unit;

  ///the value that the indicator will show
  double _data;

  ///the date that the indicator will show
  DateTime _date;

  ///the color of the indicator
  Color _colore;

  ///the width of the indicator
  double _width;

  ///the height of the indicator
  double _height;

  ///the callBack function that permit to change the showed data
  Function _selectData;

  ///create and return all the temperature indicator section 
  ///including the part with all the neccessary information 
  ///such as:
  ///1. the value
  ///2. the unit of measurment
  ///3. the date of that value
  ///4. the two button to move around the chart
  CurrentTempData(String unit, double data, DateTime date, Color color, double width, double height, {Function datoSelezionato}){
    this._unit = unit;
    this._data = data;
    this._date = date;

    this._colore = color;
    this._width = width;
    this._height = height;
    
    this._selectData = datoSelezionato;

  }

  @override
  _CurrentTempDataState createState() => _CurrentTempDataState();
}

class _CurrentTempDataState extends State<CurrentTempData> {

  
  @override
  void initState() {
    
    super.initState();
  }

  ///invole the callback [_selectData] function and pass it -1 to select the previous data
  void goBack(){
      widget._selectData(-1);
  }

  ///invole the callback [_selectData] function and pass it +1 to select the following data
  void goForward(){
    widget._selectData(1);
  }


  @override
  Widget build(BuildContext context) {
    return Container(

      child: Column(

        children: <Widget>[
          Expanded(
            flex: 8,
            //_ animation
            child:  TweenAnimationBuilder(
              duration: const Duration(seconds: 1),
              curve: Curves.easeOutQuad,
              tween: Tween<double>(begin: 0, end: this.widget._data),
              builder: (BuildContext context, double  value, Widget child) {

                return Container(//_ indicator
                  child: CustomPaint(
                    painter: Termometer(value, 40, -30, widget._colore),//IndicatoreAnalogico(_value, 135, 270, 100, Colors.red),
                    size: Size(widget._width, widget._height),

                    child: Container(
                      width: widget._width,
                      height: widget._height,

                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.end,

                        children: <Widget>[
                          Container(//_inner part of the indicator 

                            child: Column(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children: <Widget>[
                                Text(
                                  "${widget._date.toString().split(" ")[0]} value",
                                  style: TextStyle(fontSize: 18),
                                ),

                                Row(
                                  mainAxisAlignment: MainAxisAlignment.center,
                                  children: <Widget>[
                                    Text(//_ part that show the value
                                      "${double.parse(value.toStringAsFixed(1))}",
                                      style: TextStyle(fontSize: 60),
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
                        ],
                      ),
                    ),
                  ),

                  alignment: Alignment.center,
                );


              },
            ),

          ),

          Expanded(
            flex: 2,
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[

                FlatButton(//_ goBack button
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