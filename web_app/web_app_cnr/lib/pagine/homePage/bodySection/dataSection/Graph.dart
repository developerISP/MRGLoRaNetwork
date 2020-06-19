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
  factory DateTimeLineChart.withDate({List<double> data, List<DateTime> date, double selData, DateTime selDate, color}) {
    return new DateTimeLineChart(
      _createData(data, date, selData, selDate, color),
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
          new charts.LineRendererConfig(includeArea: false, stacked: false),

      
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
  static List<charts.Series<Point, DateTime>> _createData(data, date, selData, selDate, color) {

    final graphData = [
      
      for(int i=0; i<data.length; i++) 
        fit(data[i], date[i])
    ];

    final selectedData = [fit(selData, selDate)];

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

  ///Trasform the given data and dates into a Point object
  static Point fit(double data, DateTime date){ 
    return Point(new DateTime(date.year, date.month, date.day, date.hour), data);
  }
}


