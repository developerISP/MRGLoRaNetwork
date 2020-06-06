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

  ///the list of the points of the line chart
  List<Data> _data;

  ///the color of the indicator
  Color _colore;

  ///the width of the indicator
  double _width;

  ///the height of the indicator
  double _height;

  ///the callBack function that permit to change the showed data
  Function _selectData;

  ///the index of the showed data
  int _index;

  ///the value at the respective [_index]
  double _finalValue;

  ///create and return all the temperature indicator section 
  ///including the part with all the neccessary information 
  ///such as:
  ///1. the value
  ///2. the unit of measurment
  ///3. the date of that value
  ///4. the two button to move around the chart
  CurrentTempData(String unit, List<Data> data, Color color, double width, double height, int index, {Function datoSelezionato}){
    this._unit = unit;
    this._data = data;
    this._colore = color;
    this._width = width;
    this._height = height;
    
    this._selectData = datoSelezionato;

    this._index = index;
    this._finalValue = this._data[this._index].value;
  }

  @override
  _CurrentTempDataState createState() => _CurrentTempDataState();
}

class _CurrentTempDataState extends State<CurrentTempData> {

  ///initialize: [_index] at the last index of the [_data] list
  ///and [_finalValue] with the value at [_index]
  @override
  void initState() {
    //_ by default the [_index] is set at the end of the [_data] list
    this.widget._index = widget._data.length-1;
    this.widget._finalValue = widget._data[this.widget._index].value;
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
              tween: Tween<double>(begin: 0, end: this.widget._finalValue),
              builder: (_, _value, __) {

                return Container(//_ indicator
                  child: CustomPaint(
                    painter: Termometer(_value, 100, widget._colore),//IndicatoreAnalogico(_value, 135, 270, 100, Colors.red),
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
                                  (this.widget._index == widget._data.length-1 ? "today's" : widget._data[this.widget._index].date) + " value",
                                  style: TextStyle(fontSize: 18),
                                ),

                                Row(
                                  mainAxisAlignment: MainAxisAlignment.center,
                                  children: <Widget>[
                                    Text(//_ part that show the value
                                      "${_value.toInt()}",
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