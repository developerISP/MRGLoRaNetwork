import 'package:flutter/material.dart';
import 'pagine/homePage/HomePage.dart';

void main() {
  runApp(MyApp());
}

///create the web application
class MyApp extends StatelessWidget {

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'CNR project',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: HomePage(),
    );
  }
}
