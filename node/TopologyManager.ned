// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package node;

//
// TODO auto-generated type
//
simple TopologyManager
{
    parameters:
        @display("p=73,174;i=block/topo_l");
        int numPaths;
        int assignedRoute;
    gates:
        input in @directIn;
        output directTopoOut @directOut;
}
