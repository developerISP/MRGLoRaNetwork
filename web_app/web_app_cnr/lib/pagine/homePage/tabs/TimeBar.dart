import 'package:flutter/material.dart';
import '../HomePage.dart' show Data, Intervall;

///generate a timeBar to define the starting and the ending point
///of the time intervall to show on the chart
class TimeBar extends StatefulWidget {
  ///the date of the starting point of the intervall
  Data startDate;

  ///the date of the ending point of the intervall
  Data endDate;

  ///The variable for the intervall
  Intervall intervall;

  ///The variable for the line chart
  List<Data> data;

  ///the callBack function to update the intervall in the homePage
  Function update; 

  ///generate a timeBar to define the starting and the ending point
  ///of the time intervall to show on the chart
  TimeBar({this.startDate, this.endDate, this.intervall, this.data, this.update});

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

  ///Change the starting point of the intervall
  void changeStart(int delta){
    setState(() {
      widget.intervall.min = widget.intervall.min + delta < 0
      ? 0
      : widget.intervall.min + delta >= widget.intervall.max-2
        ? widget.intervall.max-2
        : widget.intervall.min + delta;

      widget.startDate = widget.data[widget.intervall.min];
    });
  }

  ///Change the finish point of the intervall
  void changeFinish(int delta){
    setState(() {
      widget.intervall.max = widget.intervall.max + delta <= widget.intervall.min+2
      ? widget.intervall.min+2
      : widget.intervall.max + delta > widget.data.length
        ? widget.data.length
        : widget.intervall.max + delta;

      widget.endDate = widget.data[widget.intervall.max-1];
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
            child: Text(" "+widget.startDate.date+" ", style: TextStyle(fontSize: 17)),
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
            child: Text(" "+widget.endDate.date+" ", style: TextStyle(fontSize: 17))
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
