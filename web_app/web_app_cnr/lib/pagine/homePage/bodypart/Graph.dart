import 'dart:math';

/// Example of a combo time series chart with two series rendered as lines, and
/// a third rendered as points along the top line with a different color.
///
/// This example demonstrates a method for drawing points along a line using a
/// different color from the main series color. The line renderer supports
/// drawing points with the "includePoints" option, but those points will share
/// the same color as the line.
import 'package:charts_flutter/flutter.dart' as charts;
import 'package:flutter/material.dart' hide Color;

class DateTimeComboLinePointChart extends StatelessWidget {
  final List<charts.Series> seriesList;
  final bool animate;
  final charts.Color colore;

  DateTimeComboLinePointChart(this.seriesList, {this.animate, this.colore});

  /// Creates a [TimeSeriesChart] with sample data and no transition.
  factory DateTimeComboLinePointChart.withDate(data, selData, colore) {
    return new DateTimeComboLinePointChart(
      _createData(data, selData, colore),
      colore: colore,
      // Disable animations for image tests.
      animate: false,
    );
  }

  @override
  Widget build(BuildContext context) {
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



  /// Create one series with sample hard coded data.
  static List<charts.Series<TimeSeries, DateTime>> _createData(data, selData, colore) {
    final graphData = [
      for(int i=0; i<data.length; i++) 
        _adattamento(data[i])
    ];

    final selectedData = [_adattamento(selData)];

    return [
      new charts.Series<TimeSeries, DateTime>(
        id: 'Graph',
        colorFn: (_, __) => colore,
        areaColorFn: (_, __) => colore.lighter,
        

        domainFn: (TimeSeries sales, _) => sales.time,
        measureFn: (TimeSeries sales, _) => sales.value,
        data: graphData,
      ),
      new charts.Series(
        id: "selectedData",

        colorFn: (_, __) => colore,
        
        domainFn: (TimeSeries sales, _) => sales.time,
        measureFn: (TimeSeries sales, _) => sales.value, 

        data: selectedData,
      )..setAttribute(charts.rendererIdKey, "customPoint")
      
    ];
  }
}

/// Sample time series data type.
class TimeSeries {
  final DateTime time;
  final double value;

  TimeSeries(this.time, this.value);
}



TimeSeries _adattamento(data){
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
  
  return TimeSeries(new DateTime(year, month, day, /*hour*/), value);
}