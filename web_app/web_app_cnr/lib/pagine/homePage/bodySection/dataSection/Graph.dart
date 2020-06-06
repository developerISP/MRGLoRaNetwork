import 'dart:math';

import 'package:charts_flutter/flutter.dart' as charts;
import 'package:flutter/material.dart' hide Color;


///Define the points in the chart
class Point {
  final DateTime time;
  final double value;

  Point(this.time, this.value);
}

///Define a Line chart, with date and time, in the X axis
///and the value in the Y axis
class DateTimeLineChart extends StatelessWidget {
  ///contain all the intersections points, between X and Y axis, of the chart
  final List<charts.Series> seriesList;

  ///indicate if the chart is animated or not
  final bool animate;

  ///the color of the chart
  final charts.Color color;

  ///the defalut constructor
  DateTimeLineChart(this.seriesList, {this.animate, this.color});

  ///the constructor that create the chart at the given data and color
  ///
  ///SelData indicate the point showed by the indicator
  factory DateTimeLineChart.withDate(data, selData, color) {
    return new DateTimeLineChart(
      _createData(data, selData, color),
      color: color,

      // Disable animations.
      animate: false,
    );
  }

  @override
  Widget build(BuildContext context) {

    //_ creating the graph
    return new charts.TimeSeriesChart(
      
      seriesList,
      animate: animate,
      
      defaultRenderer:
          new charts.LineRendererConfig(includeArea: true, stacked: false),

      
      customSeriesRenderers: [
          new charts.PointRendererConfig(
            customRendererId: 'customPoint')
      ]
    );
  }

  /// Create one series with the given parameters
  /// 
  /// this methos use [fit] to trasform all data in a array of [Point]
  /// ```dart
  /// graphData = [
  ///    for(int i=0; i<data.length; i++) 
  ///     fit(data[i])
  ///  ];
  /// ```
  /// 
  /// and then return the object that will be used to render the chart
  static List<charts.Series<Point, DateTime>> _createData(data, selData, color) {
    final graphData = [
      for(int i=0; i<data.length; i++) 
        fit(data[i])
    ];

    final selectedData = [fit(selData)];

    return [
      new charts.Series<Point, DateTime>(
        id: 'Graph',
        colorFn: (_, __) => color,
        areaColorFn: (_, __) => color.lighter,
        

        domainFn: (Point sales, _) => sales.time,
        measureFn: (Point sales, _) => sales.value,
        data: graphData,
      ),
      new charts.Series(
        id: "selectedData",

        colorFn: (_, __) => color,
        
        domainFn: (Point sales, _) => sales.time,
        measureFn: (Point sales, _) => sales.value, 

        data: selectedData,
      )..setAttribute(charts.rendererIdKey, "customPoint")
      
    ];
  }

  //todo forse andrà adattato per un array di struct invece che per un array di stringhe
  ///Trasform the given string into a Point object
  ///
  ///this method split the string to get every single field of the information
  ///and use it to construct and return the Point object
  ///
  ///the format of the string data is: dd/mm/aaaa and value
  static Point fit(data){
    double value = 0;
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;

    //_il formato del dato è dd/mm/aaaa e value
    value = data.value;

    List<String> date = data.date.split('/');

    day = int.parse(date[0]);
    month = int.parse(date[1]);
    year = int.parse(date[2]);
    //hour = int.parse(date[3]);
    
    return Point(new DateTime(year, month, day, /*hour*/), value);
  }
}


