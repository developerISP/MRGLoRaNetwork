import 'package:charts_flutter/flutter.dart' as charts;
import 'package:flutter/material.dart';
import 'dataSection/datiCorrenti/CurrentTempData.dart';
import 'dataSection/datiCorrenti/CurrentOzonData.dart';

import '../HomePage.dart' show Data;

import 'dataSection/Graph.dart';


///define the body section of the home page
///
///this class contain a line chart and a "Precision" indicator for the:
///1. ozon data
///2. temperature data
///3. umidity data
///4. wind data (strength and direction)
class BodySection extends StatefulWidget {

  ///the list of the points of the line chart
  List<Data> _data;

  ///the index of the ozon data showed in the indicator
  int _ozonIndex;

  ///the index of the temperature data showed in the indicator
  int _tempIndex;

  ///the index of the umidity data showed in the indicator
  int _umidiyIndex;

  ///the index of the wind data showed in the indicator
  int _windIndex;


  ///the ozon data at the respective [_ozonIndex]
  Data _ozonSelectedData;

  ///the temperature data at the respective [_tempIndex]
  Data _tempSelectedData;

  ///the umidity data at the respective [_umidiyIndex]
  Data _umiditySelectedData;

  ///the wind data at the respective [_windIndex]
  Data _windSelectedData;

  ///Define the ozon section data
  ///
  ///the ozonSection is composed by:
  ///1. *title*
  ///2. *data section*, which contain
  ///   1. *line chart* (from class DateTimeComboLinePointChart)
  ///   2. *indicator* (from class CurrentOzonData)
  /// 
  /// see also:
  /// * other charts from [charts_flutter](https://pub.dev/packages/charts_flutter)
  Container ozonSection;

  ///Define the temp section data
  ///
  ///the temperatureSection is composed by:
  ///1. *title*
  ///2. *data section*, which contain
  ///   1. *indicator* (from class CurrentTempData)
  ///   2. *line chart* (from class DateTimeComboLinePointChart)
  /// 
  /// see also:
  /// * other charts from [charts_flutter](https://pub.dev/packages/charts_flutter)
  Container temperatureSection;

  ///Define the umidity section data
  ///
  ///the UmiditySection is composed by:
  ///1. *title*
  ///2. *data section*, which contain
  ///   2. *line chart* (from class DateTimeComboLinePointChart)
  ///   1. *indicator* (from class CurrentUmidityData)
  /// 
  /// see also:
  /// * other charts from [charts_flutter](https://pub.dev/packages/charts_flutter)
  Container umiditySection;

  ///Define the wind section data
  ///
  ///the windSection is composed by:
  ///1. *title*
  ///2. *data section*, which contain
  ///   1. *indicator* (from class CurrentWindData)
  ///   2. *line chart* (from class DateTimeComboLinePointChart)
  /// 
  /// see also:
  /// * other charts from [charts_flutter](https://pub.dev/packages/charts_flutter)
  Container windSection;

  ///define the body section of the home page
  ///
  ///this class contain a line chart and a "Precision" indicator for the:
  ///1. ozon data
  ///2. temperature data
  ///3. umidity data
  ///4. wind data (strength and direction)
  BodySection({data}){
    this._data = data;

    this._ozonIndex = this._data.length-1;
    this._tempIndex = this._data.length-1;
    this._umidiyIndex = this._data.length-1;
    this._windIndex = this._data.length-1;

    this._ozonSelectedData = this._data[this._ozonIndex];
    this._tempSelectedData = this._data[this._tempIndex];
    this._umiditySelectedData = this._data[this._umidiyIndex];
    this._windSelectedData = this._data[this._windIndex];
  }

  @override
  _BodySectionState createState() => _BodySectionState();
}


class _BodySectionState extends State<BodySection>{
  
  
  @override
  void initState() {
    this.widget._ozonIndex = this.widget._data.length-1;

    this.widget._ozonSelectedData = this.widget._data[this.widget._ozonIndex];
    this.widget._tempSelectedData = this.widget._data[this.widget._tempIndex];
    this.widget._umiditySelectedData = this.widget._data[this.widget._umidiyIndex];
    this.widget._windSelectedData = this.widget._data[this.widget._windIndex];
    

    super.initState();
  }


  @override
  Widget build(BuildContext context) {
    
    double width = 250;
    double height = 250;

    double dataSectionHeight = 400;

    //+_ Ozon section
    widget.ozonSection = Container(
      margin: EdgeInsets.fromLTRB(20, 0, 0, 20),

      child: Column(
        children: <Widget>[

          //_ title
          Container(
            margin: EdgeInsets.fromLTRB(0, 20, 0, 0),
            child: Center(child: Text("Ozon data", style: TextStyle(fontSize: 20)))
          ),

          //_ data
          Container(
            margin: EdgeInsets.fromLTRB(0, 0, 0, 20),

            height: dataSectionHeight,

            child: Row(
              children: <Widget>[

                //_ line chart
                Expanded(
                  flex: 6,
                  child: Center(child: DateTimeLineChart.withDate(widget._data, this.widget._ozonSelectedData, charts.MaterialPalette.blue.shadeDefault))
                ),

                //_ indicator
                Expanded(
                  flex: 4,
                  child: CurrentOzonData(
                    "mg/l",
                    widget._data,
                    Colors.lightBlue,
                    width,
                    height,
                    widget._ozonIndex,
                    
                    selectData: (int deltaIndex){
                      setState(() {
                        //set the ozonIndex to 0 if the sum between ozonIndex and deltaIndex is reaching the low limit
                        if(widget._ozonIndex + deltaIndex < 0) widget._ozonIndex = 0;
                        //set the ozonIndex to _data.length-1 if the sum between ozonIndex and deltaIndex is reaching the top limit
                        else if(widget._ozonIndex + deltaIndex >= widget._data.length) widget._ozonIndex = widget._data.length-1;
                        //if neighter the two condition before are true then the ozonIndex will increase or decrease
                        else widget._ozonIndex += deltaIndex;

                        widget._ozonSelectedData = this.widget._data[widget._ozonIndex];
                        
                      });
                      
                    },

                  ),
                )


              ],
            ),
          ),

        ],
      ),

    );
    //+_ Ozon section

    //+_ Temperature section
    widget.temperatureSection = Container(
      margin: EdgeInsets.fromLTRB(20, 0, 0, 20),

      child: Column(
        children: <Widget>[

          Container(//_ title
            margin: EdgeInsets.fromLTRB(0, 20, 0, 0),
            child: Center(child: Text("Temperature data", style: TextStyle(fontSize: 20)))
          ),

          Container(//_ data
            margin: EdgeInsets.fromLTRB(0, 0, 0, 20),

            height: dataSectionHeight,

            child: Row(
              children: <Widget>[

                Expanded(//_ indicator
                  flex: 4,
                  child: CurrentTempData(
                    "°C",
                    widget._data,
                    Colors.red,
                    width,
                    height,
                    widget._tempIndex,

                    datoSelezionato: (int indice){
                      setState(() {
                        //set the tempIndex to 0 if the sum between tempIndex and deltaIndex is reaching the low limit
                        if(widget._tempIndex + indice < 0) widget._tempIndex = 0;
                        //set the tempIndex to _data.length-1 if the sum between tempIndex and deltaIndex is reaching the top limit
                        else if(widget._tempIndex + indice >= widget._data.length) widget._tempIndex = widget._data.length-1;
                        //if neighter the two condition before are true then the tempIndex will increase or decrease
                        else widget._tempIndex += indice;

                        widget._tempSelectedData = this.widget._data[widget._tempIndex];
                        
                      });
                      
                    }

                  )
                  
                ),

                Expanded(//_ chart
                  flex: 6,
                  child: Center(child: DateTimeLineChart.withDate(widget._data, this.widget._tempSelectedData, charts.MaterialPalette.red.shadeDefault))
                ),



              ],
            ),
          ),

        ],
      ),
    );
    //+_ Temperature section

    //+_ Umidity section 
    widget.umiditySection = Container(
      margin: EdgeInsets.fromLTRB(20, 0, 0, 20),

      child: Column(
        children: <Widget>[

          Container(//_ title
            margin: EdgeInsets.fromLTRB(0, 20, 0, 0),
            child: Center(child: Text("Umidity data", style: TextStyle(fontSize: 20)))
          ),

          Container(//_ data
            margin: EdgeInsets.fromLTRB(0, 0, 0, 20),

            height: dataSectionHeight,

            child: Row(
              children: <Widget>[

                Expanded(//_ chart
                  flex: 6,
                  child: Center(child: DateTimeLineChart.withDate(widget._data, this.widget._umiditySelectedData, charts.MaterialPalette.yellow.shadeDefault))
                ),

                Expanded(//_ indicator
                  flex: 4,
                  child: CurrentOzonData(
                    "%",
                    widget._data,
                    Colors.amber,
                    width,
                    height,
                    widget._umidiyIndex,

                    selectData: (int indice){
                      setState(() {
                        //set the umidityIndex to 0 if the sum between umidityIndex and deltaIndex is reaching the low limit
                        if(widget._umidiyIndex + indice < 0) widget._umidiyIndex = 0;
                        //set the umidityIndex to _data.length-1 if the sum between umidityIndex and deltaIndex is reaching the top limit
                        else if(widget._umidiyIndex + indice >= widget._data.length) widget._umidiyIndex = widget._data.length-1;
                        //if neighter the two condition before are true then the umidityIndex will increase or decrease
                        else widget._umidiyIndex += indice;

                        widget._umiditySelectedData = this.widget._data[widget._umidiyIndex];
                        
                      });
                      
                    }
                    
                  )
                  
                ),


              ],
            ),
          ),


        ],
      ),

    );
    //+_ Umidity section

    //+_ Wind section
    widget.windSection = Container(
      margin: EdgeInsets.fromLTRB(20, 0, 0, 20),

      child: Column(
        children: <Widget>[

          Container(//_ title
            margin: EdgeInsets.fromLTRB(0, 20, 0, 0),
            child: Center(child: Text("Wind data", style: TextStyle(fontSize: 20)))
          ),

          Container(//_ data
            margin: EdgeInsets.fromLTRB(0, 0, 0, 20),

            height: dataSectionHeight,

            child: Row(
              children: <Widget>[

                Expanded(//_ indicator
                  flex: 4,
                  child: CurrentOzonData(
                    "km/h",
                    widget._data,
                    Colors.green,
                    width,
                    height,
                    widget._windIndex,

                    selectData: (int indice){
                      setState(() {
                        //set the windIndex to 0 if the sum between windIndex and deltaIndex is reaching the low limit
                        if(widget._windIndex + indice < 0) widget._windIndex = 0;
                        //set the windIndex to _data.length-1 if the sum between windIndex and deltaIndex is reaching the top limit
                        else if(widget._windIndex + indice >= widget._data.length) widget._windIndex = widget._data.length-1;
                        //if neighter the two condition before are true then the windIndex will increase or decrease
                        else widget._windIndex += indice;

                        widget._windSelectedData = this.widget._data[widget._windIndex];
                        
                      });
                      
                    }

                  )
                    
                ),

                Expanded(//_ chart
                  flex: 6,
                  child: Center(child: DateTimeLineChart.withDate(widget._data, this.widget._windSelectedData, charts.MaterialPalette.green.shadeDefault))
                ),

              ],
            ),
          ),


        ],
      )

    );
    //+_ Wind section


    return Column(
      children: <Widget>[

        widget.ozonSection,

        widget.temperatureSection,

        widget.umiditySection,

        widget.windSection


      ],
    );
  }
}
