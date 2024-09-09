#include <stdlib.h>
#include <math.h>


#define TF_LITE_STATIC_MEMORY

#include "tensorflow/lite/core/c/common.h"
#include "models/kwsint8_model.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#ifdef _GEM5_
#include <gem5/m5ops.h>
#endif /*_GEM5_*/


#define INPUT_SIZE 49*10

float example_input [INPUT_SIZE] =  {-27.415142059326172, -3.050898313522339, 2.9281671047210693, 1.1546016931533813, -1.1645299196243286, 0.9835680723190308, -0.900667130947113, 0.9502159357070923, 0.30075177550315857, -1.9989254474639893, -30.542573928833008, -1.8740872144699097, 3.3789749145507812, 1.4192880392074585, 1.0833885669708252, -0.1490655094385147, 0.17711789906024933, 1.2933498620986938, 0.9479442238807678, -1.3928533792495728, -29.625091552734375, -1.0786516666412354, 3.6749460697174072, -0.41315290331840515, 0.47316214442253113, 0.13897141814231873, 0.8765814304351807, 0.20278477668762207, 1.1896613836288452, -0.8912652134895325, -29.758169174194336, 0.4909486472606659, 2.4400694370269775, -0.6584120392799377, 0.14202401041984558, 0.30913910269737244, 0.7355331778526306, -0.003780941478908062, 1.5882471799850464, -0.099278524518013, -30.121585845947266, -0.9594947099685669, 0.9314965009689331, 0.42072588205337524, 2.1864309310913086, -0.7984405755996704, -1.1327913999557495, -0.3694601356983185, 0.3447556495666504, 0.4055326581001282, -31.221214294433594, 1.5392627716064453, 0.3504653573036194, -0.6326335072517395, 2.903898000717163, 1.2011501789093018, -0.29723015427589417, -1.0011106729507446, 0.16726630926132202, 0.2745310962200165, -27.820531845092773, 0.9749978184700012, -3.2535557746887207, -2.250001907348633, 2.5972440242767334, 1.8638436794281006, -0.0803370475769043, -1.4856815338134766, -0.28435584902763367, 0.15294593572616577, -22.165443420410156, 0.8823047280311584, -3.6548259258270264, -3.0373477935791016, 3.555910110473633, 3.12035870552063, 0.5376484990119934, -1.0044641494750977, -0.06892500817775726, -0.1383659988641739, -12.311931610107422, 3.3364875316619873, -2.2668402194976807, -3.3070642948150635, 5.026087760925293, 3.659303903579712, -0.3720814883708954, -1.2364771366119385, -0.8201778531074524, -0.8381315469741821, 4.05674934387207, 5.828392028808594, 0.5922868847846985, -4.498016357421875, 3.4150209426879883, 1.9988702535629272, -0.7872309684753418, -0.6586981415748596, 0.15023165941238403, -2.1207876205444336, 15.479952812194824, 4.013426303863525, -0.712415337562561, -4.827877044677734, 1.7329002618789673, 2.6271023750305176, -1.9883180856704712, 0.24328583478927612, -0.6581146121025085, -0.863871157169342, 19.116241455078125, 3.1812386512756348, -0.4283251166343689, -3.983226776123047, 1.4403778314590454, 2.5783443450927734, -2.227607250213623, 0.0717288926243782, -0.6994757652282715, -0.3391512334346771, 18.721363067626953, 2.9042770862579346, -0.8561766743659973, -3.6952502727508545, 1.49916672706604, 2.1128673553466797, -0.9794716835021973, -0.8793714642524719, -0.26725196838378906, -0.8566879630088806, 18.18943977355957, 3.270587682723999, -0.9624758362770081, -3.6161718368530273, 1.70549476146698, 2.490746259689331, -1.150674819946289, -0.046954263001680374, -0.3022702634334564, -0.4158308207988739, 18.952661514282227, 3.345266103744507, -1.3876657485961914, -3.4258766174316406, 1.5766420364379883, 2.0713539123535156, -1.085513710975647, 0.1680036187171936, -0.6807762384414673, -0.36754128336906433, 18.817298889160156, 3.4338512420654297, -2.197397232055664, -3.407827377319336, 1.1411560773849487, 1.8981791734695435, -0.9960311651229858, -0.0986623615026474, -0.47626638412475586, -1.0472767353057861, 16.436304092407227, 3.0219409465789795, -1.7022563219070435, -2.868051290512085, 1.7003637552261353, 1.9247814416885376, -0.585319459438324, 0.35506704449653625, 0.4489672780036926, -0.7665953040122986, 5.745650291442871, 4.760601997375488, 0.39803746342658997, -1.5433670282363892, 1.8835550546646118, 2.1169121265411377, -0.2503066658973694, -0.7798848748207092, -0.5049693584442139, -1.9563512802124023, 4.171435832977295, -0.3539217710494995, -0.19973134994506836, -0.8752108812332153, 0.5461021661758423, 0.7213212847709656, 1.1984561681747437, 0.90048748254776, -0.6845646500587463, -0.24588130414485931, 3.904780149459839, -4.674875259399414, 0.020460031926631927, -0.10572177916765213, 0.5204997062683105, 0.9193880558013916, -0.12024788558483124, -0.23568026721477509, -0.08264729380607605, -0.08660389482975006, 1.5845630168914795, -6.641822814941406, 1.0133707523345947, 0.09212902933359146, 0.659741997718811, 0.7901103496551514, 0.040725648403167725, -0.11563650518655777, -0.683584451675415, -0.3216158151626587, -0.48241764307022095, -6.423887252807617, 0.647883415222168, 0.30363157391548157, 0.7476341128349304, 0.041718438267707825, -1.0023154020309448, 0.48678287863731384, -0.8996714949607849, -0.6871751546859741, -2.0830867290496826, -5.861123085021973, 0.5808992981910706, -0.4913642406463623, 0.5138222575187683, 0.19352704286575317, 0.10739699751138687, -0.17124530673027039, -0.6934924125671387, 0.355937123298645, -7.9773993492126465, -7.445985794067383, 0.5062808990478516, 0.44687893986701965, -0.17551420629024506, -0.13523298501968384, -0.2995494604110718, -0.10738354176282883, 0.060547761619091034, -0.3909030258655548, -15.394425392150879, -6.2793989181518555, 2.7904229164123535, 1.23860764503479, -0.1382734775543213, 0.3002507984638214, -0.06176324933767319, 0.6908859610557556, -0.07638629525899887, 0.6988946795463562, -16.869548797607422, -6.854589462280273, 2.246838331222534, 1.1906293630599976, 0.27928704023361206, -0.3844500482082367, -0.49906495213508606, 0.7411397695541382, -0.22463369369506836, 0.20203040540218353, -12.446207046508789, -7.331985950469971, 2.340283155441284, 1.3255457878112793, 2.1276819705963135, 0.11614710092544556, 0.2285495400428772, 1.0195839405059814, -0.061376310884952545, -0.3836274743080139, -4.288690090179443, -7.335846424102783, -0.6169227361679077, -0.48062917590141296, 0.18164397776126862, 0.6705648303031921, 1.2748477458953857, 0.39295417070388794, -0.7674293518066406, 0.18004527688026428, -7.24304723739624, -2.2098042964935303, -1.564144492149353, 0.5894036889076233, 1.531422734260559, 0.8100572824478149, -0.07488930225372314, -1.0128819942474365, -0.3367134928703308, -0.16338762640953064, -11.143895149230957, -0.062037672847509384, -3.209195137023926, -0.39148077368736267, 1.3583614826202393, -0.6047162413597107, 0.11337189376354218, -1.5030227899551392, 0.14903779327869415, -1.8328299522399902, -13.95045280456543, -1.3341025114059448, -3.944664716720581, 0.492307186126709, 2.2340121269226074, -0.6879815459251404, 0.2569445073604584, -0.6612923741340637, 0.3357936441898346, -2.1993398666381836, -17.980579376220703, -1.1580734252929688, -3.959437608718872, -0.0002180461451644078, 3.0156660079956055, -0.11606033891439438, 0.5752983689308167, -0.8571560382843018, -0.377472460269928, -1.062439203262329, -20.11602210998535, -1.2066758871078491, -3.1056811809539795, 2.02817964553833, 2.853120803833008, -0.711884617805481, -0.2549792230129242, -0.0249628983438015, -0.6393707990646362, -2.275956392288208, -22.903039932250977, -1.2102322578430176, -3.0188798904418945, 0.9648112058639526, 2.9657888412475586, -1.2820277214050293, 0.523617684841156, 1.175065279006958, -0.5481598973274231, -0.9915156364440918, -25.480375289916992, -0.6570876836776733, -0.4637608528137207, 1.516026258468628, 1.8591630458831787, -1.8895317316055298, 0.26504406332969666, 1.089581847190857, -0.2649518549442291, -0.48777103424072266, -25.802175521850586, 0.09058329463005066, 0.9203222990036011, 2.0729098320007324, 0.8338356018066406, -1.6638667583465576, 0.9505431056022644, 0.8700644373893738, -1.2535196542739868, -0.12094118446111679, -28.292428970336914, 0.2308269441127777, -0.23325926065444946, 2.2577438354492188, 0.8864670395851135, -1.1496583223342896, 0.2388058304786682, 0.7183875441551208, -1.5112619400024414, -0.8699957728385925, -27.738544464111328, 0.20008938014507294, -0.3072229027748108, 1.743322730064392, 1.4049609899520874, -0.6685701012611389, -0.01246260292828083, 0.4636757969856262, -2.141155242919922, -0.4091605544090271, -28.747575759887695, 0.7701948881149292, 0.5237762928009033, 1.1190848350524902, 0.7207143902778625, -1.0649871826171875, 0.3716810345649719, 0.6651753783226013, -1.0768821239471436, 0.8149757385253906, -30.11128807067871, 0.5166720151901245, 1.433642864227295, 0.009253792464733124, -0.13193219900131226, -0.7546584010124207, 1.1559319496154785, 0.8454295992851257, -0.4582326412200928, -0.1606997847557068, -31.536102294921875, 0.09804735332727432, 1.5461705923080444, 1.936417579650879, 0.9206430315971375, -0.44333839416503906, -0.6205846667289734, -0.583767831325531, -0.3578754663467407, 0.7743569016456604, -30.368885040283203, 1.2969253063201904, 1.1964455842971802, 1.0748459100723267, 0.16334140300750732, -0.6644306182861328, -0.2678621709346771, -0.5665125846862793, -0.4902551472187042, 0.6062992215156555, -30.703372955322266, 0.8593543171882629, 1.0198557376861572, 0.9743595719337463, 0.2033432275056839, 0.24969865381717682, 0.6850486397743225, -0.4129834771156311, 0.013416501693427563, -0.6590796709060669, -31.6260929107666, -0.23029564321041107, 0.0957251712679863, 0.21121804416179657, 0.5146448612213135, -0.35293081402778625, -0.3811420500278473, -0.6882553100585938, 0.48716965317726135, 0.5100531578063965, -31.258617401123047, 0.09324777871370316, 0.5735539197921753, 1.0799641609191895, 1.3245512247085571, 0.5534435510635376, -0.3319275677204132, 0.4059343636035919, 1.106203317642212, -0.04595503583550453, -30.575820922851562, 0.4202771484851837, 0.5778854489326477, -0.060589052736759186, 0.6092408895492554, -0.22942495346069336, -0.282884418964386, -0.418312668800354, 0.11825484782457352, 0.050362154841423035, -30.68138313293457, 0.48645368218421936, 1.1071326732635498, -0.1497403383255005, -0.2921597957611084, 0.19598530232906342, 0.26167115569114685, 0.7467785477638245, -0.33491167426109314, 0.0698784589767456, -31.59876251220703, -0.5213159322738647, 0.5358129143714905, 0.741112470626831, 0.7555860280990601, 0.20080092549324036, 0.5504661798477173, -0.622554361820221, -0.8328073024749756, -0.8010011911392212, -31.93376350402832, 0.5031962394714355, 1.1932412385940552, 0.34729474782943726, -0.4512538015842438, -0.42770421504974365, 0.535057008266449, -0.8714216947555542, -0.361982524394989, 0.033541467040777206}; 



namespace {
using OpResolver = tflite::MicroMutableOpResolver<7>;

TfLiteStatus RegisterOps(OpResolver& op_resolver) {
  TF_LITE_ENSURE_STATUS(op_resolver.AddFullyConnected());
  TF_LITE_ENSURE_STATUS(op_resolver.AddRelu());
  TF_LITE_ENSURE_STATUS(op_resolver.AddConv2D());
  TF_LITE_ENSURE_STATUS(op_resolver.AddDepthwiseConv2D());
  TF_LITE_ENSURE_STATUS(op_resolver.AddAveragePool2D());
  TF_LITE_ENSURE_STATUS(op_resolver.AddReshape());
  TF_LITE_ENSURE_STATUS(op_resolver.AddSoftmax());
  return kTfLiteOk;
}
#define MODEL kwsint8_tflite
#define ARENA_SIZE 22896
}

int main() {
tflite::InitializeTarget();
const tflite::Model* model = tflite::GetModel(MODEL);
OpResolver op_resolver;
RegisterOps(op_resolver);
constexpr int kTensorArenaSize = ARENA_SIZE;
uint8_t tensor_arena[kTensorArenaSize];

tflite::MicroInterpreter interpreter(model, op_resolver, tensor_arena, kTensorArenaSize);
interpreter.AllocateTensors();
auto input = interpreter.input_tensor(0);
std::copy_n(example_input, INPUT_SIZE, tflite::GetTensorData<uint8_t>(input));
/********************************************************
 *****      Starting Region of Interest (ROI)    ********
********************************************************/
#if _GEM5_
 m5_work_begin(1,1);   
#endif /*_GEM5_*/


interpreter.Invoke();

/********************************************************
 *****      End Region of Interest (ROI)         ********
********************************************************/
#ifdef _GEM5_
    m5_work_end(1,1);
#endif /*_GEM5_*/
    return 0;
}

