import 'package:flutter/material.dart';

class TerzoTab extends StatelessWidget {
  const TerzoTab({Key key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("progetto CNR", style: TextStyle(fontSize: 20)),
        elevation: 0,
      ),
      body: Column(
        children: <Widget>[
          Expanded(
            child: Center(child: Icon(Icons.local_pizza, size: 200, color: Colors.deepOrange,)),
          )
          
        ],
      ),

    );
    
  }
}