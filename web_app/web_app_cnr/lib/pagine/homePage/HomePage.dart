import 'dart:math';

import 'package:flutter/material.dart';
import 'tabs/TimeBar.dart';


import 'bodySection/BodySection.dart';
import 'drawer/DrawerPart.dart';

//!IMPORTANTE  questa classe è qui temporaneamente per il testing
///define the variable for the line chart
class Data{
  String date;
  double value;

  Data(this.date, this.value);
}

///Define the intervall of the showed data
class Intervall{
  int min;
  int max;

  Intervall(this.min, this.max);
}

///Define che home page of the web application
///
///this class contain the AppBar and the BodySection
class HomePage extends StatefulWidget {

  HomePage({Key key}) : super(key: key);

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {

  ///The variable for the line chart
  List<Data> data;
  ///The variable for the intervall
  Intervall intervall;

  Future<dynamic> bottomSheetOutput;

  
  ///Initialize [data] and [intervall]
  @override
  void initState() {
    //initialization of this.data
    this.data = [
      for(int i=1; i<=20; i++)
        new Data("$i/05/2020", Random().nextInt(100).toDouble()),
    ];
    //initialization of intervall
    this.intervall = Intervall(
      this.data.length - 15 - 1 < 0
      ? 0
      : this.data.length - 15 - 1,
      this.data.length,
    );

    super.initState();
  }

  ///the callBack function that update the time intervall when the botton sheet is closed
  void update(int min, int max){
    setState(() {
      intervall.min = min;
      intervall.max = max;
    });
    //intervall.min = min;
    //intervall.max = max;
  }

  ///this function generate a bottom sheet when the button on the appBar is pressed
  void onButtonPressed(){

    TimeBar tb = TimeBar(
      startDate: this.data[intervall.min],
      endDate: this.data[intervall.max-1],
      intervall: intervall,
      data: data,
      update: this.update,
    );

    bottomSheetOutput = showModalBottomSheet(
      context: context, 
      enableDrag: false,
      builder: (context){
        return tb;
      }
    ).then((value) => tb.updateDate() );


  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      drawer: Drawer(
        child: DrawerPart()

      ),

      //_ title
      appBar: AppBar(
        title: Text("CNR project", style: TextStyle(fontSize: 24),),
        centerTitle: true,

        
        actions: <Widget>[
          //_ button for the intervall definition
          FlatButton(
            child: Row(
              children: <Widget>[ Icon(Icons.chevron_right, color: Colors.white,), Icon(Icons.chevron_left, color: Colors.white) ],
              mainAxisAlignment: MainAxisAlignment.center,
            ),

            onPressed: () => onButtonPressed(),
          )

          //_ button for the project info
          /* FlatButton(
            child: Text(
              "project info",
              style: TextStyle(
                fontSize: 15,
                color: Colors.white,
              ),
            ),
            //! questo link dovrebbe portare alle inforazione del progetto
            onPressed: null
          ) */
        ],
        
        elevation: 0,
      ),



      //_body section of the page
      body: ListView(
        children: <Widget>[

          BodySection(
            data: this.data.sublist(intervall.min, intervall.max),
          ),

        ],
      )
    );
  }
}