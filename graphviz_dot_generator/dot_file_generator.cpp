// void create_dot_file(vector<user> users){
//     string result = "digraph Graph {\n\n";
//     result += "\tnode [ shape = \"record\"  color = \"red\"]\n\n";
//     for(const auto &user : users)
//     {
//         result += "\t" + to_string(user.id) + " [ label = \"{ " + user.name + +" | id = " + to_string(user.id) +
//             " }\" ]\n";
//         result += "\t" + to_string(user.id) + " -> {";
//         for(size_t i = 0; i < user.followers.size(); i++)
//         {
//             if(i)
//                 result += " ,";
//             result += to_string(user.followers[i]);
//         }
//         result += "}\n";
//     }
//     result += "}\n\n";


//     QFile file("graph.dot");
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
//              return;
//     }
//     QTextStream out(&file);
//     QString text = QString::fromStdString(result);
//     out << text;
//     file.flush();
//     file.close();
//     system("dot -Tpng -O graph.dot");
// }
