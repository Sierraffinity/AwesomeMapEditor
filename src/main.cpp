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
// Copyright (C) 2016 Diegoisawesome, Pokedude
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
#include <AME/Forms/MainWindow.h>
#include <QApplication>
#include <QDebug>


///////////////////////////////////////////////////////////
/// \brief Main entry point
///
/// This function creates an object of type 'MainWindow',
/// binds it to the current application and shows it.
///
/// \param argc Argument count
/// \param argv Argument array
///
///////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QApplication a(argc, argv);
	QApplication::setOrganizationName("DoMoreAwesome");
	QApplication::setOrganizationDomain("domoreaweso.me");
	QApplication::setApplicationName("Awesome Map Editor");
	QApplication::setApplicationDisplayName("Awesome Map Editor");
	QApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("Awesome Map Editor - A GBA Pokémon game map editor.");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", QObject::tr("The ROM file to open."));

	parser.addOptions({ { { "m", "map" }, QObject::tr("Bank and map to initially load."), QObject::tr("map") }, });

	parser.process(a);

	const QStringList args = parser.positionalArguments();

    ame::MainWindow w;
    w.show();

	if (args.length() > 0 && w.loadROM(args.at(0)))
	{
		w.loadMapData();

        QStringList mapAndBank = parser.value("m").split(",");
        bool okBank = false;
        bool okMap = false;
        int bank = 0;
        int map = 0;

        if (mapAndBank.size() == 2)
        {
            bank = mapAndBank[0].toInt(&okBank, 0x10);
            map = mapAndBank[1].toInt(&okMap, 0x10);
        }

        if (okBank && okMap)
            w.loadMapChangeTreeView(bank, map);
        //else
        //    Messages::showError(NULL, QObject::tr("Invalid map specified."));
	}

    return a.exec();
}
