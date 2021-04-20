// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package node;

//
// TODO auto-generated type
//
simple TopologyManager
{
    parameters:
        int numPaths;
        int assignedRoute;
    gates:
        input in @directIn;
        output directTopoOut @directOut;
}
