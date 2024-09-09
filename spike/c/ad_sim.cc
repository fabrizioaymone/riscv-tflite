#include <math.h>
#include <iostream>

#define TF_LITE_STATIC_MEMORY

#include "tensorflow/lite/core/c/common.h"
#include "models/adfp32_model.h"
#include "models/adint8_model.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"


#define INPUT_SIZE 640
#define OUTPUT_SIZE 640


float example_input [INPUT_SIZE] =  {-42.94598389, -17.1367836 ,  -8.797225  ,  -1.85389829, -0.38880891,   1.2928952 ,  -6.85299444,  -5.16505432, -13.98539734, -12.81475163,  -7.74158239,  -4.95257902, -3.81015182,  -9.30844498,  -7.45835304,  -3.14719009, -12.17734528, -13.51252937, -12.532341  ,  -9.46300411, -7.14234304,  -9.23156929, -20.52666664, -18.89893532, -16.80979538, -18.91579437, -17.93419647, -21.92086411, -17.08174515, -12.90128326, -16.62577438, -15.5206871 , -11.04250145, -13.62853432, -21.90981674, -20.70177841, -19.28282166, -21.23855972, -23.37163162, -25.59459305, -33.96525955, -18.03845596, -18.74704552, -28.15924835, -27.14559174, -24.10575485, -28.36016655, -28.93803406, -30.11473846, -25.25715637, -24.79143906, -29.63569832, -27.05565643, -21.76935196, -26.83929634, -29.25071526, -30.97217369, -27.05812073, -25.33736038, -28.35795593, -31.15777588, -30.6957531 , -30.88023949, -29.77928543, -33.40498352, -31.55140305, -31.49746704, -36.47184753, -31.01928329, -27.90036392, -34.10679245, -35.02571869, -32.25560379, -29.05171394, -28.08091354, -30.29251099, -32.26486588, -33.7906456 , -31.37770844, -34.98276138, -37.63028336, -38.36217499, -40.81445694, -39.87580872, -36.28592682, -35.8142128 , -35.68494034, -41.00254059, -41.35843277, -37.31593323, -35.09999084, -34.32925415, -37.68226242, -41.22631073, -37.77242661, -36.11926651, -37.51060867, -38.99516296, -41.89628601, -43.60442352, -39.8093605 , -37.6293602 , -39.68147278, -41.27045441, -40.54611969, -46.81439972, -39.94369888, -40.95939636, -40.51102066, -44.29504395, -43.18071365, -43.89524078, -40.24515152, -43.38225555, -44.50039673, -43.82287598, -46.3872757 , -44.83144379, -45.91024399, -43.22310638, -43.37161636, -43.8153038 , -43.3773613 , -42.45930481, -45.46352005, -50.16910172, -52.79483032, -66.04699707, -41.06686401, -21.82773781, -13.04864693,  -3.13680911, 0.94391334,   1.41352463,  -3.86636353,  -0.51924551, -6.15964222, -11.90701294,  -2.93750811,  -1.7709868 , -3.2128253 ,  -6.98524475,  -9.93388748, -10.38224411, -9.77413368,  -5.09100628,  -6.2616663 , -11.31324673, -19.11518097, -14.4993639 , -14.97669792, -13.08625317, -18.73616791, -15.62945747, -20.04146576, -13.11140537, -9.96993542, -12.11150551, -22.25592422, -20.2240181 , -19.01420021, -15.68520355, -15.47133827, -19.8227005 , -27.09660339, -20.0295372 , -21.59671021, -22.03255081, -21.65902328, -14.00444603, -18.14788818, -29.52017784, -25.18104553, -27.18876266, -25.90269852, -34.41232681, -30.63920593, -25.4122963 , -28.76668167, -27.82240868, -25.02923584, -24.45163536, -26.15924454, -27.51724434, -27.90849495, -28.27601242, -32.6313858 , -32.79419327, -28.80267334, -29.93293953, -33.6287117 , -29.03409576, -29.62065125, -33.16494751, -30.74315071, -27.54625511, -31.41720581, -32.16094589, -32.65788651, -32.78810501, -28.53900528, -25.66082954, -30.64831924, -28.23838997, -30.6052742 , -35.32469177, -29.59800339, -30.89349747, -35.86100769, -36.46538544, -46.39977264, -39.49242401, -36.88936996, -34.68202209, -33.18374252, -34.31564331, -38.75931931, -36.37394714, -34.21462631, -37.02662659, -35.7732048 , -41.03446198, -34.93441772, -35.93717957, -38.77428818, -37.27246475, -36.19583511, -41.76334381, -40.3372612 , -39.8514061 , -40.04082108, -40.35354996, -41.64300919, -42.18544769, -45.88103485, -43.92517471, -42.95973969, -39.2233696 , -42.18465424, -41.24930954, -43.23838806, -43.93985748, -41.59867859, -44.82880402, -47.45725632, -46.24242783, -47.32132339, -45.9947052 , -43.27600098, -45.4740715 , -42.26956177, -41.8883934 , -42.59460449, -45.28411102, -52.0218277 , -65.35210419, -40.13286591, -26.53590012, -14.35064697,  -2.71413541, -0.98009706,  -4.37560558,  -6.79103279,  -1.27205026, -0.84771454,  -7.64123344,  -9.85926437,  -4.52041578, -8.3411665 , -13.54094315,  -9.04278183, -10.01033306, -11.20645046, -11.37550926, -14.54139233, -12.81772995, -13.96288776, -16.00918961,  -6.43230152,  -6.28786325, -17.53427887, -18.37205124, -23.80709839, -17.55916595, -18.19052124, -17.12758446, -16.77593994, -20.30153656, -18.08611679, -17.63300705, -14.90660286, -16.17333221, -20.89948845, -14.625494  , -12.56111908, -15.31199265, -21.92264938, -14.54532146, -19.09972382, -30.32292938, -23.89349365, -24.29668427, -31.37907982, -28.72769928, -31.15413666, -29.16215897, -26.18384361, -25.32290649, -26.81383896, -33.01869202, -25.59588242, -18.22729492, -25.63169289, -30.0331974 , -36.47939301, -38.26411819, -42.77193451, -35.27867508, -32.92115021, -36.32616425, -32.99790955, -33.01535797, -37.12378693, -33.89469528, -29.85584641, -31.92332268, -37.88396454, -37.62116241, -35.96266937, -31.54485893, -29.73371887, -31.89794159, -30.96449089, -37.07514572, -33.9058609 , -30.4278717 , -32.08343124, -33.82038879, -37.49483109, -36.99524307, -40.26347733, -42.42841339, -40.15524292, -40.41041946, -39.86075211, -43.4945488 , -39.71088791, -33.01556396, -36.83422852, -43.9291687 , -38.20462036, -38.14465714, -40.02586365, -40.53608322, -40.45011139, -43.90179443, -41.07859802, -37.57822418, -37.84916306, -42.06452942, -41.05912018, -46.86771011, -44.33808136, -40.10379791, -39.43166351, -42.74050903, -43.39426422, -40.78552246, -42.97981644, -45.19496155, -42.90229034, -42.96860504, -46.84186554, -46.68484116, -47.86685181, -45.20097733, -45.69130707, -48.42604065, -43.68582153, -42.9891777 , -46.06196976, -45.90893173, -52.07460785, -66.04155731, -47.87695312, -27.9522934 , -15.8178997 ,  -9.61010933, -6.03289557,  -6.22957325,  -6.13715935,   1.23252833, -2.43922186, -10.44041061,  -4.75940037,  -3.18321848, -9.40049076, -14.80436516, -19.37434387, -14.78285027, -15.49308777, -13.70228004, -15.75088024, -13.66803265, -10.10980511, -14.30265331, -12.25175762,  -9.1181097 , -20.68698502, -30.98274612, -25.33435822, -19.01892471, -21.1292057 , -19.45012283, -18.87220001, -15.08521557, -14.88949966, -19.47061157, -20.93638229, -19.14009857, -23.42194557, -13.5899353 , -12.14211941, -14.68259239, -19.2254734 , -17.48398781, -22.01558876, -23.55485535, -27.03454018, -31.19636345, -27.57393837, -28.12701225, -31.60795212, -30.68817329, -25.43634415, -25.06617737, -29.94459915, -32.48274231, -35.53877258, -29.87809181, -26.39672279, -25.13465881, -31.61207199, -37.00473404, -33.68378067, -30.72188377, -28.86646652, -27.46508598, -30.99395561, -34.41726685, -32.3939209 , -33.64225388, -34.48297119, -28.88097954, -32.80435181, -35.73500824, -34.28561783, -31.80761147, -30.54369164, -33.4552536 , -35.68209076, -35.32625198, -32.42663574, -29.73768425, -34.16763687, -38.68751907, -37.87374878, -35.93593597, -39.11117172, -36.51723862, -40.67066193, -41.21115112, -36.81204605, -38.75349426, -39.81464386, -34.21590424, -35.74338913, -43.91279221, -36.56444168, -38.29947662, -41.15377426, -38.37372208, -37.30711365, -40.2567215 , -40.22225952, -41.89896393, -38.49952698, -42.75550079, -43.22633743, -43.94309616, -45.4765358 , -42.51934052, -39.1570816 , -41.53790283, -39.57817459, -42.09764099, -41.5912056 , -41.7389679 , -41.67858124, -45.53784943, -44.82772446, -46.16722107, -42.63004684, -41.7542038 , -43.13976288, -44.76676559, -44.41807175, -43.08026123, -43.66352081, -46.67001724, -55.0807724 , -65.70426178, -42.17670441, -20.8115654 , -14.72624588, -12.7006321 , -6.76946592,  -1.19813311, -10.15579128, -14.79079723, -17.18803024, -10.78387451, -11.36421776,  -8.58532333, -8.954916  ,  -6.84870481,  -7.1031189 , -10.17271233, -13.62098885, -14.46333981, -13.81486034, -12.41600513, -9.33119011,  -9.91369438, -16.00312996, -14.90439606, -20.11655426, -20.60294342, -20.65088081, -21.31046677, -26.52742577, -26.91761971, -20.69854736, -21.46637344, -19.86563873, -16.32472038, -13.18670654, -16.58176804, -22.91245651, -13.43697357, -15.00545692, -22.00497627, -22.51023293, -15.22746086, -16.79003143, -24.96147346, -26.43228912, -29.60719681, -30.34299469, -31.72364807, -28.30157852, -26.71596527, -33.51889801, -31.516819  , -27.84145164, -29.19989014, -32.75699615, -28.61387253, -26.58924484, -32.99787521, -34.19456863, -32.49051666, -29.78681183, -27.89799881, -29.47324371, -33.82157516, -33.27490234, -32.15233612, -33.192173  , -37.1952095 , -37.42168045, -31.88209724, -33.58134842, -35.43418503, -36.93511963, -33.80212021, -34.236763  , -32.86579132, -35.140625  , -30.48690414, -32.04568481, -33.77931595, -35.44820404, -39.24723816, -38.89321518, -41.39362335, -44.10407257, -40.68862534, -39.22959137, -44.09962463, -42.22174072, -38.85440826, -37.19260788, -33.59145355, -37.10141754, -42.62153625, -38.68087387, -37.0294075 , -37.90450287, -37.69223785, -39.77088165, -39.42676163, -38.91416168, -38.93970108, -38.78926849, -41.08219147, -42.49454117, -42.3712883 , -39.93291092, -39.42088699, -45.26538849, -40.42844391, -42.65802383, -43.46208572, -43.63409042, -42.05094147, -40.20407867, -39.83572388, -40.76248169, -41.1905365 , -47.93330383, -43.77215958, -42.10821152, -44.90959167, -42.86006165, -42.51334763, -43.68756866, -46.35308838, -50.80653763, -65.89585876};


namespace {
using OpResolver = tflite::MicroMutableOpResolver<2>;

TfLiteStatus RegisterOps(OpResolver& op_resolver) {
  TF_LITE_ENSURE_STATUS(op_resolver.AddFullyConnected());
  TF_LITE_ENSURE_STATUS(op_resolver.AddRelu());
  return kTfLiteOk;
}
#define FP32_MODEL adfp32_tflite
#define INT8_MODEL adint8_tflite
#define FP32_ARENA_SIZE 4745
#define INT8_ARENA_SIZE 3400
}  // namespace

#define read_csr(reg) ({ unsigned long __tmp;asm volatile ("csrr %0, " #reg : "=r"(__tmp));__tmp; })
#define CSR_INSTRET 0xc02



TfLiteStatus ProfileMemoryAndLatencyFloat() {
  tflite::MicroProfiler profiler;
  OpResolver op_resolver;
  TF_LITE_ENSURE_STATUS(RegisterOps(op_resolver));

  // Arena size just a round number. The exact arena usage can be determined
  // using the RecordingMicroInterpreter.
  constexpr int kTensorArenaSize =FP32_ARENA_SIZE;
  uint8_t tensor_arena[kTensorArenaSize];
  constexpr int kNumResourceVariables = 24;

  tflite::RecordingMicroAllocator* allocator(
      tflite::RecordingMicroAllocator::Create(tensor_arena, kTensorArenaSize));
  tflite::RecordingMicroInterpreter interpreter(
      tflite::GetModel(FP32_MODEL), op_resolver, allocator,
      NULL,
      &profiler);

  TF_LITE_ENSURE_STATUS(interpreter.AllocateTensors());
  TFLITE_CHECK_EQ(interpreter.inputs_size(), 1);
  interpreter.input(0)->data.f[INPUT_SIZE] = 1.f;
  TF_LITE_ENSURE_STATUS(interpreter.Invoke());

  MicroPrintf("");  // Print an empty new line
  profiler.LogTicksPerTagCsv();

  MicroPrintf("");  // Print an empty new line
  interpreter.GetMicroAllocator().PrintAllocations();
  return kTfLiteOk;
}




TfLiteStatus ProfileMemoryAndLatencyQuant() {
  tflite::MicroProfiler profiler;
  OpResolver op_resolver;
  TF_LITE_ENSURE_STATUS(RegisterOps(op_resolver));

  // Arena size just a round number. The exact arena usage can be determined
  // using the RecordingMicroInterpreter.
  constexpr int kTensorArenaSize =INT8_ARENA_SIZE;
  uint8_t tensor_arena[kTensorArenaSize];
  constexpr int kNumResourceVariables = 24;

  tflite::RecordingMicroAllocator* allocator(
      tflite::RecordingMicroAllocator::Create(tensor_arena, kTensorArenaSize));
  tflite::RecordingMicroInterpreter interpreter(
      tflite::GetModel(INT8_MODEL), op_resolver, allocator,
      NULL,
      &profiler);

  TF_LITE_ENSURE_STATUS(interpreter.AllocateTensors());
  TFLITE_CHECK_EQ(interpreter.inputs_size(), 1);
  interpreter.input(0)->data.f[INPUT_SIZE] = 1.f;
  TF_LITE_ENSURE_STATUS(interpreter.Invoke());

  MicroPrintf("");  // Print an empty new line
  profiler.LogTicksPerTagCsv();

  MicroPrintf("");  // Print an empty new line
  interpreter.GetMicroAllocator().PrintAllocations();
  return kTfLiteOk;
}



TfLiteStatus LoadFloatModelAndPerformInference() {
    MicroPrintf("");  // Print an empty new line
  const tflite::Model* model =
      tflite::GetModel(FP32_MODEL);
  TFLITE_CHECK_EQ(model->version(), TFLITE_SCHEMA_VERSION);

  OpResolver op_resolver;
  TF_LITE_ENSURE_STATUS(RegisterOps(op_resolver));

  // Arena size just a round number. The exact arena usage can be determined
  // using the RecordingMicroInterpreter.
  constexpr int kTensorArenaSize = FP32_ARENA_SIZE;
  uint8_t tensor_arena[kTensorArenaSize];

  tflite::MicroInterpreter interpreter(model, op_resolver, tensor_arena,
                                       kTensorArenaSize);
  TF_LITE_ENSURE_STATUS(interpreter.AllocateTensors());

    // Obtain pointers to the model’s input and output tensors.
    // These return null no matter how they are typed
    auto input = interpreter.input_tensor(0);
    auto output = interpreter.output_tensor(0);

    auto inData = input->data.data;
    auto outData = output->data.data;

    printf("Input pointer %i, Output pointer %i\n",inData,outData);
    printf("Input type %i, Output type %i\n",input->type,output->type);
    printf("Input typename %s, Output typename %s\n",TfLiteTypeGetName(input->type),TfLiteTypeGetName(output->type));
    printf("Input bytes %i, Output bytes %i\n",input->bytes,output->bytes);


    std::copy_n(example_input, INPUT_SIZE, tflite::GetTensorData<float>(input));

    interpreter.input(0)->data.f = example_input;

    unsigned long instret1;
    unsigned long instret2;
    instret1 = read_csr(instret);
    interpreter.Invoke();
    instret2 = read_csr(instret);
    std::cout <<"This is instret1 "<< instret1 << std::endl;
    std::cout <<"This is instret2 "<< instret2 << std::endl;
    std::cout <<"This is difference "<< instret2-instret1 << std::endl;
    
    float y_pred[OUTPUT_SIZE];
    std::copy_n(tflite::GetTensorData<float>(output), OUTPUT_SIZE, y_pred);

    for (int i=0; i <OUTPUT_SIZE; i++) 
    std::cout << y_pred[i] << " " ;
    std::cout << std::endl;
    fflush(stdout);

  return kTfLiteOk;
}

TfLiteStatus LoadQuantModelAndPerformInference() {
    MicroPrintf("");  // Print an empty new line
  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model =
      ::tflite::GetModel(INT8_MODEL);
  TFLITE_CHECK_EQ(model->version(), TFLITE_SCHEMA_VERSION);

  OpResolver op_resolver;
  TF_LITE_ENSURE_STATUS(RegisterOps(op_resolver));

  // Arena size just a round number. The exact arena usage can be determined
  // using the RecordingMicroInterpreter.
  constexpr int kTensorArenaSize = INT8_ARENA_SIZE;
  uint8_t tensor_arena[kTensorArenaSize];

  tflite::MicroInterpreter interpreter(model, op_resolver, tensor_arena,
                                       kTensorArenaSize);

  TF_LITE_ENSURE_STATUS(interpreter.AllocateTensors());

      // Obtain pointers to the model’s input and output tensors.
    // These return null no matter how they are typed
    auto input = interpreter.input_tensor(0);
    auto output = interpreter.output_tensor(0);

    auto inData = input->data.data;
    auto outData = output->data.data;

    printf("Input pointer %i, Output pointer %i\n",inData,outData);
    printf("Input type %i, Output type %i\n",input->type,output->type);
    printf("Input typename %s, Output typename %s\n",TfLiteTypeGetName(input->type),TfLiteTypeGetName(output->type));
    printf("Input bytes %i, Output bytes %i\n",input->bytes,output->bytes);


    std::copy_n(example_input, INPUT_SIZE, tflite::GetTensorData<uint8_t>(input));

    TF_LITE_ENSURE_STATUS(interpreter.Invoke());
    
    uint8_t y_pred[OUTPUT_SIZE];
    std::copy_n(tflite::GetTensorData<uint8_t>(output), OUTPUT_SIZE, y_pred);

    for (int i=0; i <OUTPUT_SIZE; i++) 
    std::cout << (int) y_pred[i] << " " ;
    std::cout << std::endl;
    fflush(stdout);
    

  return kTfLiteOk;
}

int main(int argc, char* argv[]) {
  MicroPrintf("\n\n\n\n~~~AD SPIKE SIMULATION~~~\n\n\n\n");
  tflite::InitializeTarget();
  MicroPrintf("\n\n\n\n~~~FP32 MEMORY AND LATENCY~~~\n\n\n\n");
  TF_LITE_ENSURE_STATUS(ProfileMemoryAndLatencyFloat());
  MicroPrintf("\n\n\n\n~~~FP32 INFERENCE~~\n\n\n\n");
  TF_LITE_ENSURE_STATUS(LoadFloatModelAndPerformInference());
  //MicroPrintf("\n\n\n\n~~~INT8 MEMORY AND LATENCY~~~\n\n\n\n");
  //TF_LITE_ENSURE_STATUS(ProfileMemoryAndLatencyQuant());
  //MicroPrintf("\n\n\n\n~~~INT8 INFERENCE~~\n\n\n\n");
  //TF_LITE_ENSURE_STATUS(LoadQuantModelAndPerformInference());
  MicroPrintf("~~~ALL TESTS PASSED~~~\n");
  return kTfLiteOk;
}

