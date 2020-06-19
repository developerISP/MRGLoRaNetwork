import 'package:flutter/material.dart';
import '../HomePage.dart' show Data, Intervall;

///generate a timeBar to define the starting and the ending point
///of the time intervall to show on the chart
class TimeBar extends StatefulWidget {
  ///the date of the starting point of the intervall
  DateTime startDate; 

  ///the date of the ending point of the intervall
  DateTime endDate; 

  ///The variable for the intervall
  Intervall intervall; 

  Intervall intervallLimits; 

  ///the callBack function to update the intervall in the homePage
  Function update; 

  ///generate a timeBar to define the starting and the ending point
  ///of the time intervall to show on the chart
  TimeBar({DateTime startDate, DateTime endDate, Intervall intervall, Intervall intervallLimits, Function update}){
    this.intervall = intervall;
    this.update = update;
    this.intervallLimits = intervallLimits;

    this.endDate = endDate;
    this.startDate = startDate;
  }

  ///the function that was called in the homePage when we close the bottom sheet
  void updateDate(){
    this.update(intervall.min, intervall.max);
  }

  @override
  _TimeBarState createState() => _TimeBarState();
}

class _TimeBarState extends State<TimeBar> {


  @override
  void initState() {

    super.initState();
  }

  ///return the module of n
  double mod(double n){
    return n >= 0
    ? n
    : n*(-1);
  }

  ///Change the starting point of the intervall
  void changeStart(int delta){
    setState(() {
      int precIndex = widget.intervall.min;

      widget.intervall.min = widget.intervall.min + delta < widget.intervallLimits.min
      ? widget.intervallLimits.min
      : widget.intervall.min + delta >= widget.intervall.max-2
        ? widget.intervall.max-2
        : widget.intervall.min + delta;

      int deltaIndex = widget.intervall.min - precIndex;
 
      widget.startDate = deltaIndex < 0
      ? widget.startDate.subtract(Duration(days: -deltaIndex ))
      : deltaIndex != 0
        ? widget.startDate.add(Duration(days: deltaIndex))
        : widget.startDate;

    });
  }

  ///Change the finish point of the intervall
  void changeFinish(int delta){
    setState(() {
      int precIndex = widget.intervall.max;

      widget.intervall.max = widget.intervall.max + delta <= widget.intervall.min+2
      ? widget.intervall.min+2
      : widget.intervall.max + delta > widget.intervallLimits.max
        ? widget.intervallLimits.max
        : widget.intervall.max + delta;

      int deltaIndex = widget.intervall.max - precIndex;

      widget.endDate = deltaIndex < 0
      ? widget.endDate.subtract(Duration(days: -deltaIndex ))
      : deltaIndex != 0
        ? widget.endDate.add(Duration(days: deltaIndex))
        : widget.endDate;

    });
  }
  

  @override
  Widget build(BuildContext context) {

    //_ start point selection
    Container start = Container(
      constraints: BoxConstraints(maxHeight: 75, maxWidth: 300),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,

        children: <Widget>[
          Text("start", style: TextStyle(fontSize: 15, fontWeight: FontWeight.w400)),

          //_ two button
          Column(
            children: <Widget>[

              Expanded(
                flex: 5,
                child: FlatButton(
                  onPressed: (){
                    changeStart(1);
                  },
                  onLongPress: () => null,
                  child: Icon(Icons.keyboard_arrow_up),
                  splashColor: Colors.lightBlue[50],
                )
              ),

              Expanded(
                flex: 5,
                child: FlatButton(
                  onPressed: (){
                     changeStart(-1);
                  },
                  onLongPress: () => null,
                  child: Icon(Icons.keyboard_arrow_down),
                  splashColor: Colors.lightBlue[50],
                )
              ),

            ],
          ),

          //_ show the date
          Container(
            height: 65,
            
            decoration: BoxDecoration(
              borderRadius: BorderRadius.circular(10),
              boxShadow: <BoxShadow>[BoxShadow(color: Colors.black38, offset: Offset(-3, 4), blurRadius: 6)],
              color: Colors.white,
            ),

            alignment: Alignment.center,
            child: Text(" "+ widget.startDate.toString().split(" ")[0] +" ", style: TextStyle(fontSize: 17)),
          ),
          

        ],
      )
    );
  
    //_ end point selection
    Container fine = Container(
      constraints: BoxConstraints(maxHeight: 75, maxWidth: 300),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          
          //_ sohw the date
          Container(
            height: 65,
            
            decoration: BoxDecoration(
              borderRadius: BorderRadius.circular(10),
              boxShadow: <BoxShadow>[BoxShadow(color: Colors.black38, offset: Offset(-3, 4), blurRadius: 6)],
              color: Colors.white,
            ),
            
            alignment: Alignment.center,
            child: Text(" "+widget.endDate.toString().split(" ")[0]+" ", style: TextStyle(fontSize: 17))
          ),

          //_ two button
          Column(
            children: <Widget>[

              Expanded(
                flex: 5,
                child: FlatButton(
                onPressed: (){
                  changeFinish(1);
                },
                onLongPress: () => null,
                child: Icon(Icons.keyboard_arrow_up),
                splashColor: Colors.lightBlue[50],
              )),

              Expanded(
                flex: 5,
                child: FlatButton(
                onPressed: (){
                  changeFinish(-1);                  
                },
                onLongPress: () => null,
                child: Icon(Icons.keyboard_arrow_down),
                splashColor: Colors.lightBlue[50],
              )),

            ], 
          ),

          Text("end", style: TextStyle(fontSize: 15, fontWeight: FontWeight.w400)),


        ],
      )
    );
  

    return Container(
      padding: EdgeInsets.all(10),
      child: Column(
        mainAxisSize: MainAxisSize.min,
        mainAxisAlignment: MainAxisAlignment.spaceAround,


        children: <Widget>[

          Container(
            padding: EdgeInsets.fromLTRB(0, 0, 0, 10),
            child: Text(
              "select the start / end point of the intervall",
              style: TextStyle(fontSize: 18, fontWeight: FontWeight.w500),

            ),
          ),

          Row(
            mainAxisAlignment: MainAxisAlignment.center,

            children: <Widget>[

              start,
              fine

            ],
          ),

        ],
      ),

    );
  }
}
