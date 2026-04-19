import SwiftUI
import UniformTypeIdentifiers


struct ContentView: View {
    var body: some View {
        NavigationStack {
            ZStack {
                Image("back")
                    .resizable()
                    .scaledToFill()
                    .ignoresSafeArea()

                VStack(spacing: 30) {
                    NavigationLink {
                        EncryptView()
                    } label: {
                        ActionButton(title: "Encrypt", icon: "lock.fill")
                    }
                    .buttonStyle(PlainButtonStyle())

                    NavigationLink {
                        DecryptView()
                    } label: {
                        ActionButton(title: "Decrypt", icon: "lock.open.fill")
                    }
                    .buttonStyle(PlainButtonStyle())
                }
            }
        }
    }
}

struct ActionButton: View {
    let title: String
    let icon: String

    var body: some View {
        RoundedRectangle(cornerRadius: 20)
            .fill(Color.blue.opacity(0.8))
            .overlay(
                HStack {
                    Image(systemName: icon)
                        .font(.title2)
                    Text(title)
                        .font(.title2)
                        .fontWeight(.semibold)
                }
                .foregroundColor(.white)
            )
            .frame(width: 250, height: 80)
            .shadow(radius: 10)
            .overlay(
                RoundedRectangle(cornerRadius: 15)
                    .stroke(Color.white.opacity(0.3), lineWidth: 2)
            )
    }
}

struct FileDropZone: View {
    let title: String
    @Binding var fileURL: URL?
    let allowedExtensions: [String]

    @State private var isTargeted = false
    @State private var showFileImporter = false

    var body: some View {
        VStack {
            Text(title)
                .font(.headline)
                .foregroundColor(.white)

            if let url = fileURL {
                ZStack(alignment: .topTrailing) {
                    Text(url.lastPathComponent)
                        .foregroundColor(.green)
                        .padding()
                        .background(Color.black.opacity(0.5))
                        .cornerRadius(8)

                    //долбаный крестик
                    Button {
                        fileURL = nil
                    } label: {
                        Image(systemName: "xmark.circle.fill")
                            .foregroundColor(.white)
                            .background(Color.black.opacity(0.6))
                            .clipShape(Circle())
                    }
                    .buttonStyle(.plain)
                    .offset(x: 8, y: -8)
                }
            } else {
                Text("Drag & drop file here")
                    .foregroundColor(.gray)
                    .padding()
                    .frame(maxWidth: .infinity)
                    .background(isTargeted ? Color.blue.opacity(0.3) : Color.black.opacity(0.3))
                    .cornerRadius(8)
                    .overlay(
                        RoundedRectangle(cornerRadius: 8)
                            .stroke(isTargeted ? Color.blue : Color.gray, lineWidth: 2)
                    )
            }
        }
        .frame(maxWidth: .infinity)
        .padding()
        .background(Color.blue.opacity(0.2))
        .cornerRadius(12)
        .overlay(
            RoundedRectangle(cornerRadius: 12)
                .stroke(isTargeted ? Color.blue : Color.blue.opacity(0.5), lineWidth: 2)
        )
        .onTapGesture {
            showFileImporter = true
        }
        .onDrop(of: [.fileURL], isTargeted: $isTargeted) { providers in
            handleDrop(providers: providers)
        }
        .fileImporter(
            isPresented: $showFileImporter,
            allowedContentTypes: [.data],
            allowsMultipleSelection: false
        ) { result in
            switch result {
            case .success(let urls):
                if let url = urls.first,
                   allowedExtensions.contains(url.pathExtension.lowercased()) {
                    fileURL = url
                }
            case .failure(let error):
                print("import error: \(error)")
            }
        }
    }

    private func handleDrop(providers: [NSItemProvider]) -> Bool {
        guard let provider = providers.first else { return false }

        provider.loadItem(forTypeIdentifier: "public.file-url", options: nil) { item, _ in
            guard let data = item as? Data,
                  let url = URL(dataRepresentation: data, relativeTo: nil),
                  allowedExtensions.contains(url.pathExtension.lowercased()) else { return }

            DispatchQueue.main.async {
                fileURL = url
            }
        }

        return true
    }
}

#Preview {
    ContentView()
}
