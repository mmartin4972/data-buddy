const path = require('path');
const CopyPlugin = require('copy-webpack-plugin');
let background_path = path.resolve(__dirname, "src", "ts", "background");
let common_path = path.resolve(__dirname, "src", "ts", "common");
module.exports = {
   mode: "production",
   entry: {
      background: path.join(background_path, "background.ts"),
      common: path.join(common_path, "common.ts")
   },
   output: {
      path: path.join(__dirname, "dist", "js"),
      filename: "[name].bundle.js",
   },
   resolve: {
      extensions: [".ts", ".js"],
   },
   module: {
      rules: [
         {
            test: /\.tsx?$/,
            loader: "ts-loader",
            exclude: /node_modules/,
            include: [background_path, common_path],
            options: { 
               instance: 'background',
               configFile: path.join(background_path, "tsconfig.json")
            }
         }
      ],
   }
   // plugins: [ // Unfortuantely have to have copy code for each since unsure which will be run
   //  new CopyPlugin({
   //    patterns: [{
   //       from: path.resolve(__dirname, "src"), 
   //       to: path.resolve(__dirname, "dist"),
   //       globOptions: {
   //          dot: true,
   //          gitignore: true,
   //          ignore: ["**/ts/**"],
   //        }
   //       }]
   //   }),
   // ],
};
