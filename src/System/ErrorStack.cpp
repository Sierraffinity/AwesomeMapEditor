//////////////////////////////////////////////////////////////////////////////////
//
//
//                     d88b         888b           d888  888888888888
//                    d8888b        8888b         d8888  888
//                   d88''88b       888'8b       d8'888  888
//                  d88'  '88b      888 '8b     d8' 888  8888888
//                 d88Y8888Y88b     888  '8b   d8'  888  888
//                d88""""""""88b    888   '8b d8'   888  888
//               d88'        '88b   888    '888'    888  888
//              d88'          '88b  888     '8'     888  888888888888
//
//
// AwesomeMapEditor: A map editor for GBA Pokémon games.
// Copyright (C) 2016 diegoisawesome, Pokedude
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/ErrorStack.hpp>
#include <QDateTime>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Defines variables exclusively used by this code file
    //
    ///////////////////////////////////////////////////////////
    QStringList s_Stack;
    QStringList s_Methods;


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    // Comment:
    //
    // Added a conversion for template strings.
    //
    ///////////////////////////////////////////////////////////
    bool ErrorStack::add(QString method, QString error, UInt32 offset)
    {
        // If the offset is zero, converts the error string
        if (offset == 0)
            error.replace("%offset%", QString::number(offset, 16));

        // Converts the __PRETTY_FUNCTION__ macro string
        Int32 whiteSpace = method.indexOf(' ');
        Int32 firstBrack = method.indexOf('(');
        method.remove(0, whiteSpace);
        method.remove(firstBrack, 69);

        // Copies the string to the stack
        s_Stack.append(error);
        s_Methods.append(method);

        return false; // Maintain code flow
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    //
    ///////////////////////////////////////////////////////////
    const QStringList &ErrorStack::errors()
    {
        return s_Stack;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    //
    ///////////////////////////////////////////////////////////
    const QStringList &ErrorStack::methods()
    {
        return s_Methods;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    // Comment:
    //
    // Converted the string list to a single error log string
    //
    ///////////////////////////////////////////////////////////
    const QString ErrorStack::log()
    {
        // Prerequisites for the log conversion
        QString log;
        QString hash = QString('#').repeated(20) + QString('\n');
        QString title("AwesomeMapEditor: Error log file\n");
        QString date = QString("Date :") + QDate::currentDate().toString(Qt::ISODate);
        QString time = QString("Time :") + QTime::currentTime().toString(Qt::ISODate); 

        // Generates the header of the log
        log.append(hash);
        log.append("# ");
        log.append(title);
        log.append("#\n\n");
        log.append(date);
        log.append("\n");
        log.append(time);
        log.append("\n#\n");
        log.append(hash);
        log.append("\n");

        // Iterates through all the errors and appends them
        for (int i = 0; i < s_Stack.size(); i++)
        {
            const QString &error = s_Stack.at(i);
            const QString &method = s_Methods.at(i);
            int scopePos = method.indexOf("::");

            log.append(
                "Error in class '" + method.left(scopePos) + "'" +
                "in function'" + method.right(method.length()-(scopePos+2)) + "'.\n" +
                "Description:\n\n" + error + "\n\n\n"
            );
        }

        return log;
    }
}
